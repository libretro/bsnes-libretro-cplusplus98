#include "libsnes.hpp"
#include <snes/snes.hpp>

#include <nall/snes/cartridge.hpp>
#include <nall/gameboy/cartridge.hpp>
using namespace nall;

void SNES::Interface::video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan) {
  unsigned width = hires ? 512 : 256;
  unsigned height = overscan ? 239 : 224;
  if(interlace) height <<= 1;
  data += 9 * 1024;  //skip front porch
  pvideo_refresh(data, width, height);
  pinput_poll();
}

void SNES::Interface::audio_sample(int16_t left, int16_t right) {
  return paudio_sample(left, right);
}

int16_t SNES::Interface::input_poll(bool port, SNES::Input::Device::e device, unsigned index, unsigned id) {
  return pinput_state(port, (unsigned)device, index, id);
}

void SNES::Interface::message(const string &text) {
  fprintf(stderr, "%s\n", (const char*)text);
}

string SNES::Interface::path(SNES::Cartridge::Slot::e slot, const string &hint) {
  return string(basename, hint);
}

SNES::Interface::Interface() : pvideo_refresh(0), paudio_sample(0), pinput_poll(0), pinput_state(0) {}

static SNES::Interface interface;

const char* snes_library_id(void) {
  static string id(SNES::Info::Name, " (", SNES::Info::Profile, ") v", SNES::Info::Version);
  return (const char*)id;
}

unsigned snes_library_revision_major(void) {
  return 1;
}

unsigned snes_library_revision_minor(void) {
  return 3;
}

void snes_set_video_refresh(snes_video_refresh_t video_refresh) {
  interface.pvideo_refresh = video_refresh;
}

void snes_set_audio_sample(snes_audio_sample_t audio_sample) {
  interface.paudio_sample = audio_sample;
}

void snes_set_input_poll(snes_input_poll_t input_poll) {
  interface.pinput_poll = input_poll;
}

void snes_set_input_state(snes_input_state_t input_state) {
  interface.pinput_state = input_state;
}

void snes_set_controller_port_device(bool port, unsigned device) {
  SNES::input.connect(port, (SNES::Input::Device::e)device);
}

void snes_set_cartridge_basename(const char *basename) {
  interface.basename = basename;
}

void snes_init(void) {
  SNES::system.init(&interface);
  SNES::input.connect(0, SNES::Input::Device::Joypad);
  SNES::input.connect(1, SNES::Input::Device::Joypad);
}

void snes_term(void) {
  SNES::system.term();
}

void snes_power(void) {
  SNES::system.power();
}

void snes_reset(void) {
  SNES::system.reset();
}

void snes_run(void) {
  SNES::system.run();
}

unsigned snes_serialize_size(void) {
  return SNES::system.serialize_size;
}

bool snes_serialize(uint8_t *data, unsigned size) {
  SNES::system.runtosave();
  serializer s = SNES::system.serialize();
  if(s.size() > size) return false;
  memcpy(data, s.data(), s.size());
  return true;
}

bool snes_unserialize(const uint8_t *data, unsigned size) {
  serializer s(data, size);
  return SNES::system.unserialize(s);
}

void snes_cheat_reset(void) {
  SNES::cheat.reset();
  SNES::cheat.synchronize();
}

void snes_cheat_set(unsigned index, bool enabled, const char *code) {
  SNES::cheat[index] = code;
  SNES::cheat[index].enabled = enabled;
  SNES::cheat.synchronize();
}

bool snes_load_cartridge_normal(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size
) {
  snes_cheat_reset();
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SNESCartridge(rom_data, rom_size).xmlMemoryMap;
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, lstring( xmlrom ));
  SNES::system.power();
  return true;
}

bool snes_load_cartridge_bsx_slotted(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  snes_cheat_reset();
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SNESCartridge(rom_data, rom_size).xmlMemoryMap;
  if(bsx_data) SNES::bsxflash.memory.copy(bsx_data, bsx_size);
  string xmlbsx = (bsx_xml && *bsx_xml) ? string(bsx_xml) : SNESCartridge(bsx_data, bsx_size).xmlMemoryMap;
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, lstring( xmlrom, xmlbsx ));
  SNES::system.power();
  return true;
}

bool snes_load_cartridge_bsx(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  snes_cheat_reset();
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SNESCartridge(rom_data, rom_size).xmlMemoryMap;
  if(bsx_data) SNES::bsxflash.memory.copy(bsx_data, bsx_size);
  string xmlbsx = (bsx_xml && *bsx_xml) ? string(bsx_xml) : SNESCartridge(bsx_data, bsx_size).xmlMemoryMap;
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, lstring( xmlrom, xmlbsx ));
  SNES::system.power();
  return true;
}

bool snes_load_cartridge_sufami_turbo(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *sta_xml, const uint8_t *sta_data, unsigned sta_size,
  const char *stb_xml, const uint8_t *stb_data, unsigned stb_size
) {
  snes_cheat_reset();
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SNESCartridge(rom_data, rom_size).xmlMemoryMap;
  if(sta_data) SNES::sufamiturbo.slotA.rom.copy(sta_data, sta_size);
  string xmlsta = (sta_xml && *sta_xml) ? string(sta_xml) : SNESCartridge(sta_data, sta_size).xmlMemoryMap;
  if(stb_data) SNES::sufamiturbo.slotB.rom.copy(stb_data, stb_size);
  string xmlstb = (stb_xml && *stb_xml) ? string(stb_xml) : SNESCartridge(stb_data, stb_size).xmlMemoryMap;
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, lstring( xmlrom, xmlsta, xmlstb ));
  SNES::system.power();
  return true;
}

bool snes_load_cartridge_super_game_boy(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *dmg_xml, const uint8_t *dmg_data, unsigned dmg_size
) {
  snes_cheat_reset();
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SNESCartridge(rom_data, rom_size).xmlMemoryMap;
  if(dmg_data) {
    string xmldmg = (dmg_xml && *dmg_xml) ? string(dmg_xml) : GameBoyCartridge(dmg_data, dmg_size).xml;
    GameBoy::cartridge.load(xmldmg, dmg_data, dmg_size);
  }
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, lstring( xmlrom, "" ));
  SNES::system.power();
  return true;
}

void snes_unload_cartridge(void) {
  SNES::cartridge.unload();
}

bool snes_get_region(void) {
  return SNES::system.region.i == SNES::System::Region::NTSC ? 0 : 1;
}

uint8_t* snes_get_memory_data(unsigned id) {
  if(SNES::cartridge.loaded() == false) return 0;

  switch(id) {
    case SNES_MEMORY_CARTRIDGE_RAM:
      return SNES::cartridge.ram.data();
    case SNES_MEMORY_CARTRIDGE_RTC:
      if(SNES::cartridge.has_srtc()) return SNES::srtc.rtc;
      if(SNES::cartridge.has_spc7110rtc()) return SNES::spc7110.rtc;
      return 0;
    case SNES_MEMORY_BSX_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      return SNES::bsxcartridge.sram.data();
    case SNES_MEMORY_BSX_PRAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      return SNES::bsxcartridge.psram.data();
    case SNES_MEMORY_SUFAMI_TURBO_A_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      return SNES::sufamiturbo.slotA.ram.data();
    case SNES_MEMORY_SUFAMI_TURBO_B_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      return SNES::sufamiturbo.slotB.ram.data();
    case SNES_MEMORY_GAME_BOY_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SuperGameBoy) break;
      return GameBoy::cartridge.ramdata;

    case SNES_MEMORY_WRAM:
      return SNES::cpu.wram;
    case SNES_MEMORY_APURAM:
      return SNES::smp.apuram;
    case SNES_MEMORY_VRAM:
      return SNES::ppu.vram;
    case SNES_MEMORY_OAM:
      return SNES::ppu.oam;
    case SNES_MEMORY_CGRAM:
      return SNES::ppu.cgram;

    default:
      return NULL;
  }

  return 0;
}

unsigned snes_get_memory_size(unsigned id) {
  if(SNES::cartridge.loaded() == false) return 0;
  unsigned size = 0;

  switch(id) {
    case SNES_MEMORY_CARTRIDGE_RAM:
      size = SNES::cartridge.ram.size();
      break;
    case SNES_MEMORY_CARTRIDGE_RTC:
      if(SNES::cartridge.has_srtc() || SNES::cartridge.has_spc7110rtc()) size = 20;
      break;
    case SNES_MEMORY_BSX_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      size = SNES::bsxcartridge.sram.size();
      break;
    case SNES_MEMORY_BSX_PRAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      size = SNES::bsxcartridge.psram.size();
      break;
    case SNES_MEMORY_SUFAMI_TURBO_A_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      size = SNES::sufamiturbo.slotA.ram.size();
      break;
    case SNES_MEMORY_SUFAMI_TURBO_B_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      size = SNES::sufamiturbo.slotB.ram.size();
      break;
    case SNES_MEMORY_GAME_BOY_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SuperGameBoy) break;
      size = GameBoy::cartridge.ramsize;
      break;

    case SNES_MEMORY_WRAM:
      size = 128 * 1024;
      break;
    case SNES_MEMORY_APURAM:
      size = 64 * 1024;
      break;
    case SNES_MEMORY_VRAM:
      size = 64 * 1024;
      break;
    case SNES_MEMORY_OAM:
      size = 544;
      break;
    case SNES_MEMORY_CGRAM:
      size = 512;
      break;

    default:
      size = 0;
  }

  if(size == -1U) size = 0;
  return size;
}
