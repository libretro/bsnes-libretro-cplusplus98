#ifdef SUPERFX_CPP

SuperFXBus superfxbus;

namespace memory {
  SuperFXGSUROM gsurom;
  SuperFXGSURAM gsuram;
  SuperFXCPUROM fxrom;
  SuperFXCPURAM fxram;
}

void SuperFXBus::init() {
  map(MapMode::Direct, 0x00, 0xff, 0x0000, 0xffff, memory::memory_unmapped);

  map(MapMode::Linear, 0x00, 0x3f, 0x0000, 0x7fff, memory::gsurom);
  map(MapMode::Linear, 0x00, 0x3f, 0x8000, 0xffff, memory::gsurom);
  map(MapMode::Linear, 0x40, 0x5f, 0x0000, 0xffff, memory::gsurom);
  map(MapMode::Linear, 0x60, 0x7f, 0x0000, 0xffff, memory::gsuram);
}

//ROM / RAM access from the SuperFX CPU

uint64_t SuperFXGSUROM::size() const {
  return memory::cartrom.size();
}

uint8 SuperFXGSUROM::read(uint64_t addr) {
  while(!superfx.regs.scmr.ron && scheduler.sync.i != Scheduler::SynchronizeMode::All) {
    superfx.add_clocks(6);
    superfx.synchronize_cpu();
  }
  return memory::cartrom.read(addr);
}

void SuperFXGSUROM::write(uint64_t addr, uint8 data) {
  while(!superfx.regs.scmr.ron && scheduler.sync.i != Scheduler::SynchronizeMode::All) {
    superfx.add_clocks(6);
    superfx.synchronize_cpu();
  }
  memory::cartrom.write(addr, data);
}

uint64_t SuperFXGSURAM::size() const {
  return memory::cartram.size();
}

uint8 SuperFXGSURAM::read(uint64_t addr) {
  while(!superfx.regs.scmr.ran && scheduler.sync.i != Scheduler::SynchronizeMode::All) {
    superfx.add_clocks(6);
    superfx.synchronize_cpu();
  }
  return memory::cartram.read(addr);
}

void SuperFXGSURAM::write(uint64_t addr, uint8 data) {
  while(!superfx.regs.scmr.ran && scheduler.sync.i != Scheduler::SynchronizeMode::All) {
    superfx.add_clocks(6);
    superfx.synchronize_cpu();
  }
  memory::cartram.write(addr, data);
}

//ROM / RAM access from the S-CPU

uint64_t SuperFXCPUROM::size() const {
  return memory::cartrom.size();
}

uint8 SuperFXCPUROM::read(uint64_t addr) {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ron) {
    static const uint8_t data[16] = {
      0x00, 0x01, 0x00, 0x01, 0x04, 0x01, 0x00, 0x01,
      0x00, 0x01, 0x08, 0x01, 0x00, 0x01, 0x0c, 0x01,
    };
    return data[addr & 15];
  }
  return memory::cartrom.read(addr);
}

void SuperFXCPUROM::write(uint64_t addr, uint8 data) {
  memory::cartrom.write(addr, data);
}

uint64_t SuperFXCPURAM::size() const {
  return memory::cartram.size();
}

uint8 SuperFXCPURAM::read(uint64_t addr) {
  if(superfx.regs.sfr.g && superfx.regs.scmr.ran) return cpu.regs.mdr;
  return memory::cartram.read(addr);
}

void SuperFXCPURAM::write(uint64_t addr, uint8 data) {
  memory::cartram.write(addr, data);
}

#endif
