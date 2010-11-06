#include <snes.hpp>
#include "debug.h"

#define SYSTEM_CPP
namespace SNES {

System *system;

#include <config/config.cpp>
#include <debugger/debugger.cpp>
#include <scheduler/scheduler.cpp>

#include <video/video.cpp>
#include <audio/audio.cpp>
#include <input/input.cpp>

#include "serialization.cpp"

void System::run() {
  scheduler->sync.i = Scheduler::SynchronizeMode::None;

  scheduler->enter();
  if(scheduler->exit_reason.i == Scheduler::ExitReason::FrameEvent) {
    input->update();
    video->update();
  }
}

void System::runtosave() {
  if(CPU::Threaded == true) {
    scheduler->sync.i = Scheduler::SynchronizeMode::CPU;
    runthreadtosave();
  }

  if(SMP::Threaded == true) {
    scheduler->thread = smp->thread;
    runthreadtosave();
  }

  if(PPU::Threaded == true) {
    scheduler->thread = ppu->thread;
    runthreadtosave();
  }

  if(DSP::Threaded == true) {
    scheduler->thread = dsp->thread;
    runthreadtosave();
  }

  for(unsigned i = 0; i < cpu->coprocessors.size(); i++) {
    Processor &chip = *cpu->coprocessors[i];
    scheduler->thread = chip.thread;
    runthreadtosave();
  }
}

void System::runthreadtosave() {
  while(true) {
    scheduler->enter();
    if(scheduler->exit_reason.i == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler->exit_reason.i == Scheduler::ExitReason::FrameEvent) {
      input->update();
      video->update();
    }
  }
}

void System::init(Interface *interface_) {
   cpu = new CPU;
   ppu = new PPU;
   video = new Video;
   audio = new Audio;
   dsp = new DSP;
   input = new Input;
   smp = new SMP;
   scheduler = new Scheduler;
   cheat = new Cheat;

   bus = new Bus;
   memory::mmio = new MMIOAccess;
   memory::memory_unmapped = new UnmappedMemory;
   memory::mmio_unmapped = new UnmappedMMIO;

  interface = interface_;
  assert(interface != 0);

  supergameboy = new SuperGameBoy;
  superfx = new SuperFX;
  superfxbus = new SuperFXBus;
  sa1 = new SA1;
  sa1bus = new SA1Bus;
  dsp1 = new DSP1;
  dsp1dr = new DSP1DR;
  dsp1sr = new DSP1SR;
  dsp2 = new DSP2;
  dsp2dr = new DSP2DR;
  dsp2sr = new DSP2SR;
  dsp3 = new DSP3;
  dsp4 = new DSP4;
  msu1 = new MSU1;
  obc1 = new OBC1;
  bsxbase = new BSXBase;
  bsxcart = new BSXCart;
  bsxflash = new BSXFlash;
  cx4 = new Cx4;
  sdd1 = new SDD1;
  serial = new Serial;
  spc7110 = new SPC7110;
  spc7110mcu = new SPC7110MCU;
  spc7110dcu = new SPC7110DCU;
  spc7110ram = new SPC7110RAM;
  srtc = new SRTC;
  st0010 = new ST0010;
  st0011 = new ST0011;
  st0018 = new ST0018;


  supergameboy->init();
  superfx->init();
  sa1->init();
  bsxbase->init();
  bsxcart->init();
  bsxflash->init();
  srtc->init();
  sdd1->init();
  spc7110->init();
  cx4->init();
  dsp1->init();
  dsp2->init();
  dsp3->init();
  dsp4->init();
  obc1->init();
  st0010->init();
  st0011->init();
  st0018->init();
  msu1->init();
  serial->init();

  video->init();
  audio->init();
  input->init();
}

void System::term() {
   delete ppu;
   delete cpu;
   delete video;
   delete audio;
   delete smp;
   delete dsp;
   delete input;
   delete memory::mmio;
   delete memory::memory_unmapped;
   delete memory::mmio_unmapped;
   delete bus;
   delete scheduler;
   delete cheat;
   
   delete supergameboy;
   delete superfx;
   delete superfxbus;
   delete sa1;
   delete sa1bus;
   delete dsp1;
   delete dsp1dr;
   delete dsp1sr;
   delete dsp2;
   delete dsp2dr;
   delete dsp2sr;
   delete dsp3;
   delete dsp4;
   delete msu1;
   delete obc1;
   delete bsxbase;
   delete bsxcart;
   delete bsxflash;
   delete cx4;
   delete sdd1;
   delete serial;
   delete spc7110;
   delete spc7110mcu;
   delete spc7110dcu;
   delete spc7110ram;
   delete srtc;
   delete st0010;
   delete st0011;
   delete st0018;
}

void System::power() {
   SNES_DBG("Entering System::power()\n");
  region = config.region;
  expansion = config.expansion_port;
  if(region.i == Region::Autodetect) {
    region.i = (cartridge.region.i == Cartridge::Region::NTSC ? Region::NTSC : Region::PAL);
  }

  cpu_frequency = region.i == Region::NTSC ? config.cpu.ntsc_frequency : config.cpu.pal_frequency;
  apu_frequency = region.i == Region::NTSC ? config.smp.ntsc_frequency : config.smp.pal_frequency;

   SNES_DBG("#1\n");
  bus->power();
  for(unsigned i = 0x2100; i <= 0x213f; i++) memory::mmio->map(i, *ppu);
  for(unsigned i = 0x2140; i <= 0x217f; i++) memory::mmio->map(i, *cpu);
  for(unsigned i = 0x2180; i <= 0x2183; i++) memory::mmio->map(i, *cpu);
  for(unsigned i = 0x4016; i <= 0x4017; i++) memory::mmio->map(i, *cpu);
  for(unsigned i = 0x4200; i <= 0x421f; i++) memory::mmio->map(i, *cpu);
  for(unsigned i = 0x4300; i <= 0x437f; i++) memory::mmio->map(i, *cpu);

   SNES_DBG("#2\n");
  audio->coprocessor_enable(false);
  if(expansion.i == ExpansionPortDevice::BSX) bsxbase->enable();
  if(memory::bsxflash.data()) bsxflash->enable();
  if(cartridge.mode.i == Cartridge::Mode::Bsx) bsxcart->enable();
  if(cartridge.mode.i == Cartridge::Mode::SuperGameBoy) supergameboy->enable();

   SNES_DBG("#3\n");
  if(cartridge.has_superfx()) superfx->enable();
  if(cartridge.has_sa1()) sa1->enable();
  if(cartridge.has_srtc()) srtc->enable();
  if(cartridge.has_sdd1()) sdd1->enable();
  if(cartridge.has_spc7110()) spc7110->enable();
  if(cartridge.has_cx4()) cx4->enable();
  if(cartridge.has_dsp1()) dsp1->enable();
  if(cartridge.has_dsp2()) dsp2->enable();
  if(cartridge.has_dsp3()) dsp3->enable();
  if(cartridge.has_dsp4()) dsp4->enable();
  if(cartridge.has_obc1()) obc1->enable();
  if(cartridge.has_st0010()) st0010->enable();
  if(cartridge.has_st0011()) st0011->enable();
  if(cartridge.has_st0018()) st0018->enable();
  if(cartridge.has_msu1()) msu1->enable();
  if(cartridge.has_serial()) serial->enable();

   SNES_DBG("#4\n");
   SNES_DBG("#4.0\n");
  cpu->power();
   SNES_DBG("#4.1\n");
  smp->power();
   SNES_DBG("#4.2\n");
  dsp->power();
   SNES_DBG("#4.3\n");
  ppu->power();
   SNES_DBG("#4.4\n");

  if(expansion.i == ExpansionPortDevice::BSX) bsxbase->power();
   SNES_DBG("#4.5\n");
  if(memory::bsxflash.data()) bsxflash->power();
   SNES_DBG("#4.6\n");
  if(cartridge.mode.i == Cartridge::Mode::Bsx) bsxcart->power();
   SNES_DBG("#4.7\n");
  if(cartridge.mode.i == Cartridge::Mode::SuperGameBoy) supergameboy->power();

   SNES_DBG("#5\n");
  if(cartridge.has_superfx()) superfx->power();
  if(cartridge.has_sa1()) sa1->power();
  if(cartridge.has_srtc()) srtc->power();
  if(cartridge.has_sdd1()) sdd1->power();
  if(cartridge.has_spc7110()) spc7110->power();
  if(cartridge.has_cx4()) cx4->power();
  if(cartridge.has_dsp1()) dsp1->power();
  if(cartridge.has_dsp2()) dsp2->power();
  if(cartridge.has_dsp3()) dsp3->power();
  if(cartridge.has_dsp4()) dsp4->power();
  if(cartridge.has_obc1()) obc1->power();
  if(cartridge.has_st0010()) st0010->power();
  if(cartridge.has_st0011()) st0011->power();
  if(cartridge.has_st0018()) st0018->power();
  if(cartridge.has_msu1()) msu1->power();
  if(cartridge.has_serial()) serial->power();

   SNES_DBG("#6\n");
  if(cartridge.mode.i == Cartridge::Mode::SuperGameBoy) cpu->coprocessors.append(supergameboy);
  if(cartridge.has_superfx()) cpu->coprocessors.append(superfx);
  if(cartridge.has_sa1()) cpu->coprocessors.append(sa1);
  if(cartridge.has_msu1()) cpu->coprocessors.append(msu1);
  if(cartridge.has_serial()) cpu->coprocessors.append(serial);

   SNES_DBG("#7\n");
  scheduler->init();

   SNES_DBG("#8\n");
  input->port_set_device(0, config.controller_port1.i);
  input->port_set_device(1, config.controller_port2.i);
   SNES_DBG("#9: Updating Input\n");
  input->update();
//video->update();
   SNES_DBG("Completing System::power()\n");
}

void System::reset() {
  bus->reset();
  cpu->reset();
  smp->reset();
  dsp->reset();
  ppu->reset();

  if(expansion.i == ExpansionPortDevice::BSX) bsxbase->reset();
  if(memory::bsxflash.data()) bsxflash->reset();
  if(cartridge.mode.i == Cartridge::Mode::Bsx) bsxcart->reset();
  if(cartridge.mode.i == Cartridge::Mode::SuperGameBoy) supergameboy->reset();

  if(cartridge.has_superfx()) superfx->reset();
  if(cartridge.has_sa1()) sa1->reset();
  if(cartridge.has_srtc()) srtc->reset();
  if(cartridge.has_sdd1()) sdd1->reset();
  if(cartridge.has_spc7110()) spc7110->reset();
  if(cartridge.has_cx4()) cx4->reset();
  if(cartridge.has_dsp1()) dsp1->reset();
  if(cartridge.has_dsp2()) dsp2->reset();
  if(cartridge.has_dsp3()) dsp3->reset();
  if(cartridge.has_dsp4()) dsp4->reset();
  if(cartridge.has_obc1()) obc1->reset();
  if(cartridge.has_st0010()) st0010->reset();
  if(cartridge.has_st0011()) st0011->reset();
  if(cartridge.has_st0018()) st0018->reset();
  if(cartridge.has_msu1()) msu1->reset();
  if(cartridge.has_serial()) serial->reset();

  if(cartridge.mode.i == Cartridge::Mode::SuperGameBoy) cpu->coprocessors.append(supergameboy);
  if(cartridge.has_superfx()) cpu->coprocessors.append(superfx);
  if(cartridge.has_sa1()) cpu->coprocessors.append(sa1);
  if(cartridge.has_msu1()) cpu->coprocessors.append(msu1);
  if(cartridge.has_serial()) cpu->coprocessors.append(serial);

  scheduler->init();

  input->port_set_device(0, config.controller_port1.i);
  input->port_set_device(1, config.controller_port2.i);
  input->update();
//video->update();
}

void System::unload() {
  if(cartridge.mode.i == Cartridge::Mode::SuperGameBoy) supergameboy->unload();
}

void System::scanline() {
  video->scanline();
  if(cpu->vcounter() == 241) scheduler->exit(Scheduler::ExitReason::FrameEvent);
}

void System::frame() {
}

System::System() : interface(0) {
  region.i = Region::Autodetect;
  expansion.i = ExpansionPortDevice::None;
}

}
