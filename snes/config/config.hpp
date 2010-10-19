struct Configuration {
  Input::Device controller_port1;
  Input::Device controller_port2;
  System::ExpansionPortDevice expansion_port;
  System::Region region;

  struct CPU {
    uint64_t version;
    uint64_t ntsc_frequency;
    uint64_t pal_frequency;
    uint64_t wram_init_value;
  } cpu;

  struct SMP {
    uint64_t ntsc_frequency;
    uint64_t pal_frequency;
  } smp;

  struct PPU1 {
    uint64_t version;
  } ppu1;

  struct PPU2 {
    uint64_t version;
  } ppu2;

  struct SuperFX {
    uint64_t speed;
  } superfx;

  Configuration();
};

extern Configuration config;
