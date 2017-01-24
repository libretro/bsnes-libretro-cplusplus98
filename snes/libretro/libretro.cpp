#include "libretro.h"
#include <snes/snes.hpp>

#include <nall/snes/cartridge.hpp>
#include <nall/gameboy/cartridge.hpp>

// Special memory types.
#define RETRO_MEMORY_SNES_BSX_RAM             ((1 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_SNES_BSX_PRAM            ((2 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_SNES_SUFAMI_TURBO_A_RAM  ((3 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_SNES_SUFAMI_TURBO_B_RAM  ((4 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_SNES_GAME_BOY_RAM        ((5 << 8) | RETRO_MEMORY_SAVE_RAM)
#define RETRO_MEMORY_SNES_GAME_BOY_RTC        ((6 << 8) | RETRO_MEMORY_RTC)

// Special game types passed into retro_load_game_special().
// Only used when multiple ROMs are required.
#define RETRO_GAME_TYPE_BSX             0x101
#define RETRO_GAME_TYPE_BSX_SLOTTED     0x102
#define RETRO_GAME_TYPE_SUFAMI_TURBO    0x103
#define RETRO_GAME_TYPE_SUPER_GAME_BOY  0x104

#define RETRO_DEVICE_JOYPAD_MULTITAP       RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 0)
#define RETRO_DEVICE_LIGHTGUN_SUPER_SCOPE  RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_LIGHTGUN, 0)
#define RETRO_DEVICE_LIGHTGUN_JUSTIFIER    RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_LIGHTGUN, 1)
#define RETRO_DEVICE_LIGHTGUN_JUSTIFIERS   RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_LIGHTGUN, 2)
using namespace nall;

struct Interface : public SNES::Interface {
  retro_video_refresh_t pvideo_refresh;
  retro_audio_sample_batch_t paudio_sample;
  retro_input_poll_t pinput_poll;
  retro_input_state_t pinput_state;
  retro_environment_t penviron;
  bool overscan;

  string basename;
  uint16_t *buffer;
  SNES::Video video;

  static unsigned snes_to_retro(SNES::Input::Device::e device) {
    switch (device) {
       default:
       case SNES::Input::Device::None:       return RETRO_DEVICE_NONE;
       case SNES::Input::Device::Joypad:     return RETRO_DEVICE_JOYPAD;
       case SNES::Input::Device::Multitap:   return RETRO_DEVICE_JOYPAD_MULTITAP;
       case SNES::Input::Device::Mouse:      return RETRO_DEVICE_MOUSE;
       case SNES::Input::Device::SuperScope: return RETRO_DEVICE_LIGHTGUN_SUPER_SCOPE;
       case SNES::Input::Device::Justifier:  return RETRO_DEVICE_LIGHTGUN_JUSTIFIER;
       case SNES::Input::Device::Justifiers: return RETRO_DEVICE_LIGHTGUN_JUSTIFIERS;
    }
  }

  static SNES::Input::Device::e retro_to_snes(unsigned device) {
    switch (device) {
       default:
       case RETRO_DEVICE_NONE:                 return SNES::Input::Device::None;
       case RETRO_DEVICE_JOYPAD:               return SNES::Input::Device::Joypad;
       case RETRO_DEVICE_JOYPAD_MULTITAP:      return SNES::Input::Device::Multitap;
       case RETRO_DEVICE_MOUSE:                return SNES::Input::Device::Mouse;
       case RETRO_DEVICE_LIGHTGUN_SUPER_SCOPE: return SNES::Input::Device::SuperScope;
       case RETRO_DEVICE_LIGHTGUN_JUSTIFIER:   return SNES::Input::Device::Justifier;
       case RETRO_DEVICE_LIGHTGUN_JUSTIFIERS:  return SNES::Input::Device::Justifiers;
    }
  }

  void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan) {
    unsigned width = hires ? 512 : 256;
    unsigned height = overscan ? 239 : 224;
    unsigned pitch = 1024 >> interlace;
    if(interlace) height <<= 1;
    data += 9 * 1024;  //skip front porch

    for(unsigned y = 0; y < height; y++) {
      const uint32_t *sp = data + y * pitch;
      uint16_t *dp = buffer + y * pitch;
      for(unsigned x = 0; x < width; x++) {
        *dp++ = video.palette[*sp++];
      }
    }

    pvideo_refresh(buffer, width, height, pitch << 1);
    pinput_poll();
  }

  void audioSample(int16_t left, int16_t right)
  {
    if(paudio_sample)
    {
       const int16_t samples[2] = {left, right};
       paudio_sample(samples, 1);
    }
  }

  int16_t inputPoll(bool port, SNES::Input::Device::e device, unsigned index, unsigned id) {
    if(id > 11) return 0;

    if (device == SNES::Input::Device::Multitap && port)
      return pinput_state(index + 1, RETRO_DEVICE_JOYPAD, 0, id);
    else
      return pinput_state(port, snes_to_retro(device), index, id);
  }

  void message(const string &text) {
    print(text, "\n");
  }

  string path(SNES::Cartridge::Slot::e slot, const string &hint) {
    return string(basename, hint);
  }

  Interface() : pvideo_refresh(0), paudio_sample(0), pinput_poll(0), pinput_state(0) {
    buffer = new uint16_t[512 * 480];
    video.generate(SNES::Video::Format::RGB15);
  }

  void setCheats(const lstring &list = lstring()) {
    if(SNES::cartridge.mode.i == SNES::Cartridge::Mode::SuperGameBoy) {
      GameBoy::cheat.reset();
      foreach(code, list) {
        lstring codelist;
        codelist.split("+", code);
        foreach(part, codelist) {
          unsigned addr, data, comp;
          if(GameBoy::Cheat::decode(part, addr, data, comp)) {
            GameBoy::CheatCode code_ = { addr, data, comp };
            GameBoy::cheat.append(code_);
          }
        }
      }
      GameBoy::cheat.synchronize();
      return;
    }

    SNES::cheat.reset();
    foreach(code, list) {
      lstring codelist;
      codelist.split("+", code);
      foreach(part, codelist) {
        unsigned addr, data;
        if(SNES::Cheat::decode(part, addr, data)) {
          SNES::CheatCode code_ = { addr, data };
          SNES::cheat.append(code_);
        }
      }
    }
    SNES::cheat.synchronize();
  }

  ~Interface() {
    delete[] buffer;
  }
};

static Interface interface;

unsigned retro_api_version(void) {
  return RETRO_API_VERSION;
}

void retro_set_environment(retro_environment_t environ_cb)
{
   interface.penviron       = environ_cb;

   static const struct retro_controller_description port_1[] = {
      { "SNES Joypad", RETRO_DEVICE_JOYPAD },
      { "SNES Mouse", RETRO_DEVICE_MOUSE },
   };

   static const struct retro_controller_description port_2[] = {
      { "SNES Joypad", RETRO_DEVICE_JOYPAD },
      { "SNES Mouse", RETRO_DEVICE_MOUSE },
      { "Multitap", RETRO_DEVICE_JOYPAD_MULTITAP },
      { "SuperScope", RETRO_DEVICE_LIGHTGUN_SUPER_SCOPE },
      { "Justifier", RETRO_DEVICE_LIGHTGUN_JUSTIFIER },
      { "Justifiers", RETRO_DEVICE_LIGHTGUN_JUSTIFIERS },
   };

   static const struct retro_controller_info ports[] = {
      { port_1, 2 },
      { port_2, 6 },
      { 0 },
   };

   static const retro_subsystem_memory_info sgb_memory[] = {
      { "srm", RETRO_MEMORY_SNES_GAME_BOY_RAM },
      { "rtc", RETRO_MEMORY_SNES_GAME_BOY_RTC },
   };

   static const struct retro_subsystem_rom_info sgb_roms[] = {
      { "GameBoy", "gb|gbc", false, false, true, sgb_memory, 2 },
      { "Super GameBoy BIOS", "sfc|smc", false, false, true, NULL, 0 },
   };

   static const retro_subsystem_memory_info sufami_a_memory[] = {
      { "srm", RETRO_MEMORY_SNES_SUFAMI_TURBO_A_RAM },
   };

   static const retro_subsystem_memory_info sufami_b_memory[] = {
      { "srm", RETRO_MEMORY_SNES_SUFAMI_TURBO_B_RAM },
   };

   static const struct retro_subsystem_rom_info sufami_roms[] = {
      { "Sufami A", "sfc|smc", false, false, false, sufami_a_memory, 1 },
      { "Sufami B", "sfc|smc", false, false, false, sufami_b_memory, 1 },
      { "Sufami BIOS", "sfc|smc", false, false, true, NULL, 0 },
   };

   static const retro_subsystem_memory_info bsx_memory[] = {
      { "srm", RETRO_MEMORY_SNES_BSX_RAM },
      { "psrm", RETRO_MEMORY_SNES_BSX_PRAM },
   };

   static const struct retro_subsystem_rom_info bsx_roms[] = {
      { "BSX ROM", "bs", false, false, true, bsx_memory, 2 },
      { "BSX BIOS", "sfc|smc", false, false, true, NULL, 0 },
   };

   // OR in 0x1000 on types to remain ABI compat with the older-style retro_load_game_special().
   static const struct retro_subsystem_info subsystems[] = {
      { "Super GameBoy", "sgb", sgb_roms, 2, RETRO_GAME_TYPE_SUPER_GAME_BOY | 0x1000 }, // Super Gameboy
      { "Sufami Turbo", "sufami", sufami_roms, 3, RETRO_GAME_TYPE_SUFAMI_TURBO | 0x1000 }, // Sufami Turbo
      { "BSX", "bsx", bsx_roms, 2, RETRO_GAME_TYPE_BSX | 0x1000 }, // BSX
      { "BSX slotted", "bsxslot", bsx_roms, 2, RETRO_GAME_TYPE_BSX_SLOTTED | 0x1000 }, // BSX slotted
      { NULL },
   };

   environ_cb(RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO, (void*)subsystems);

   environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_set_video_refresh(retro_video_refresh_t video_refresh) { interface.pvideo_refresh = video_refresh; }
void retro_set_audio_sample(retro_audio_sample_t)    {  }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t audio_sample)     { interface.paudio_sample  = audio_sample; }
void retro_set_input_poll(retro_input_poll_t input_poll)          { interface.pinput_poll    = input_poll; }
void retro_set_input_state(retro_input_state_t input_state)       { interface.pinput_state   = input_state; }

void retro_set_controller_port_device(unsigned port, unsigned device) {
  if (port < 2)
    SNES::input.connect(port, Interface::retro_to_snes(device));
}

void retro_init(void) {
  SNES::interface = &interface;
  SNES::system.init();
  SNES::input.connect(SNES::Controller::Port1, SNES::Input::Device::Joypad);
  SNES::input.connect(SNES::Controller::Port2, SNES::Input::Device::Joypad);
}

void retro_deinit(void) {
  SNES::system.term();
}

void retro_reset(void) {
  SNES::system.reset();
}

void retro_run(void) {
  SNES::system.run();
}

size_t retro_serialize_size(void) {
  return SNES::system.serialize_size;
}

bool retro_serialize(void *data, size_t size) {
  SNES::system.runtosave();
  serializer s = SNES::system.serialize();
  if(s.size() > size) return false;
  memcpy(data, s.data(), s.size());
  return true;
}

bool retro_unserialize(const void *data, size_t size) {
  serializer s((const uint8_t*)data, size);
  return SNES::system.unserialize(s);
}

struct CheatList {
  bool enable;
  string code;
  CheatList() : enable(false) {}
};

static linear_vector<CheatList> cheatList;

void retro_cheat_reset(void) {
  cheatList.reset();
  interface.setCheats();
}

void retro_cheat_set(unsigned index, bool enable, const char *code) {
  cheatList[index].enable = enable;
  cheatList[index].code = code;
  lstring list;
  for(unsigned n = 0; n < cheatList.size(); n++) {
    if(cheatList[n].enable) list.append(cheatList[n].code);
  }
  interface.setCheats(list);
}

void retro_get_system_info(struct retro_system_info *info) {
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
  static string version("v085 (", SNES::Info::Profile, ")", GIT_VERSION);
  info->library_name     = "bSNES";
  info->library_version  = version;
  info->valid_extensions = "sfc|smc";
  info->need_fullpath    = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info) {
  struct retro_system_timing timing = { 0.0, 32040.5 };
  timing.fps = retro_get_region() == RETRO_REGION_NTSC ? 21477272.0 / 357366.0 : 21281370.0 / 425568.0;

  if (!interface.penviron(RETRO_ENVIRONMENT_GET_OVERSCAN, &interface.overscan))
     interface.overscan = false;

  struct retro_game_geometry geom = { 256, interface.overscan ? 239 : 224, 512, interface.overscan ? 478 : 448 };

  info->timing   = timing;
  info->geometry = geom;
}

static bool snes_load_cartridge_normal(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_bsx_slotted(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(bsx_data) SNES::bsxflash.memory.copy(bsx_data, bsx_size);
  string xmlbsx = (bsx_xml && *bsx_xml) ? string(bsx_xml) : SnesCartridge(bsx_data, bsx_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_bsx(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(bsx_data) SNES::bsxflash.memory.copy(bsx_data, bsx_size);
  string xmlbsx = (bsx_xml && *bsx_xml) ? string(bsx_xml) : SnesCartridge(bsx_data, bsx_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_sufami_turbo(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *sta_xml, const uint8_t *sta_data, unsigned sta_size,
  const char *stb_xml, const uint8_t *stb_data, unsigned stb_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(sta_data) SNES::sufamiturbo.slotA.rom.copy(sta_data, sta_size);
  string xmlsta = (sta_xml && *sta_xml) ? string(sta_xml) : SnesCartridge(sta_data, sta_size).markup;
  if(stb_data) SNES::sufamiturbo.slotB.rom.copy(stb_data, stb_size);
  string xmlstb = (stb_xml && *stb_xml) ? string(stb_xml) : SnesCartridge(stb_data, stb_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_super_game_boy(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *dmg_xml, const uint8_t *dmg_data, unsigned dmg_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(dmg_data) {
    //GameBoyCartridge needs to modify dmg_data (for MMM01 emulation); so copy data
    uint8_t *data = new uint8_t[dmg_size];
    memcpy(data, dmg_data, dmg_size);
    string xmldmg = (dmg_xml && *dmg_xml) ? string(dmg_xml) : GameBoyCartridge(data, dmg_size).markup;
    GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy, xmldmg, data, dmg_size);
    delete[] data;
  }
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, xmlrom);
  SNES::system.power();
  return true;
}

static void init_descriptors(void)
{
   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "X" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Y" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },

      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "X" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Y" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },

      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "X" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Y" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },

      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "X" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Y" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },

      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "X" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Y" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },

      { 0 },
   };

   interface.penviron(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
}

bool retro_load_game(const struct retro_game_info *info) {
   if (!info)
      return false;

  retro_cheat_reset();
  init_descriptors();

  if (info->path) {
    interface.basename = info->path;
    char *dot = strrchr(interface.basename(), '.');
    if (dot)
       *dot = '\0';
  }

  return snes_load_cartridge_normal(info->meta, (const uint8_t*)info->data, info->size);
}

bool retro_load_game_special(unsigned game_type,
      const struct retro_game_info *info, size_t num_info) {

  retro_cheat_reset();
  init_descriptors();
  if (info[0].path) {
    interface.basename = info[0].path;
    char *dot = strrchr(interface.basename(), '.');
    if (dot)
       *dot = '\0';
  }

  switch (game_type) {
     case RETRO_GAME_TYPE_BSX:
       return num_info == 2 && snes_load_cartridge_bsx(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size);
       
     case RETRO_GAME_TYPE_BSX_SLOTTED:
       return num_info == 2 && snes_load_cartridge_bsx_slotted(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size);

     case RETRO_GAME_TYPE_SUPER_GAME_BOY:
       return num_info == 2 && snes_load_cartridge_super_game_boy(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size);

     case RETRO_GAME_TYPE_SUFAMI_TURBO:
       return num_info == 3 && snes_load_cartridge_sufami_turbo(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size,
             info[2].meta, (const uint8_t*)info[2].data, info[2].size);

     default:
       return false;
  }
}

void retro_unload_game(void) {
  SNES::cartridge.unload();
}

unsigned retro_get_region(void) {
  return SNES::system.region.i == SNES::System::Region::NTSC ? 0 : 1;
}

void* retro_get_memory_data(unsigned id) {
  if(SNES::cartridge.loaded() == false) return 0;

  switch(id) {
    case RETRO_MEMORY_SAVE_RAM:
      return SNES::cartridge.ram.data();
    case RETRO_MEMORY_RTC:
      if(SNES::cartridge.has_srtc()) return SNES::srtc.rtc;
      if(SNES::cartridge.has_spc7110rtc()) return SNES::spc7110.rtc;
      return 0;
    case RETRO_MEMORY_SNES_BSX_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      return SNES::bsxcartridge.sram.data();
    case RETRO_MEMORY_SNES_BSX_PRAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      return SNES::bsxcartridge.psram.data();
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_A_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      return SNES::sufamiturbo.slotA.ram.data();
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_B_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      return SNES::sufamiturbo.slotB.ram.data();
    case RETRO_MEMORY_SNES_GAME_BOY_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SuperGameBoy) break;
      return GameBoy::cartridge.ramdata;

    case RETRO_MEMORY_SYSTEM_RAM:
      return SNES::cpu.wram;
  }

  return 0;
}

size_t retro_get_memory_size(unsigned id) {
  if(SNES::cartridge.loaded() == false) return 0;
  size_t size = 0;

  switch(id) {
    case RETRO_MEMORY_SAVE_RAM:
      size = SNES::cartridge.ram.size();
      break;
    case RETRO_MEMORY_RTC:
      if(SNES::cartridge.has_srtc() || SNES::cartridge.has_spc7110rtc()) size = 20;
      break;
    case RETRO_MEMORY_SNES_BSX_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      size = SNES::bsxcartridge.sram.size();
      break;
    case RETRO_MEMORY_SNES_BSX_PRAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      size = SNES::bsxcartridge.psram.size();
      break;
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_A_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      size = SNES::sufamiturbo.slotA.ram.size();
      break;
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_B_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      size = SNES::sufamiturbo.slotB.ram.size();
      break;
    case RETRO_MEMORY_SNES_GAME_BOY_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SuperGameBoy) break;
      size = GameBoy::cartridge.ramsize;
      break;

    case RETRO_MEMORY_SYSTEM_RAM:
      size = 128 * 1024;
      break;
  }

  if(size == -1U) size = 0;
  return size;
}

