struct Sprite {
  bool priority0_enable;
  bool priority1_enable;
  bool priority2_enable;
  bool priority3_enable;

  struct Regs {
    uint64_t priority0;
    uint64_t priority1;
    uint64_t priority2;
    uint64_t priority3;

    uint64_t base_size;
    uint64_t nameselect;
    uint64_t tiledata_addr;
    uint64_t first_sprite;

    bool main_enable;
    bool sub_enable;

    bool interlace;

    bool time_over;
    bool range_over;
  } regs;

  struct List {
    uint64_t width;
    uint64_t height;
    uint64_t x;
    uint64_t y;
    uint64_t character;
    bool use_nameselect;
    bool vflip;
    bool hflip;
    uint64_t palette;
    uint64_t priority;
    bool size;
  } list[128];
  bool list_valid;

  uint8 itemlist[32];
  struct TileList {
    uint64_t x;
    uint64_t y;
    uint64_t priority;
    uint64_t palette;
    uint64_t tile;
    bool hflip;
  } tilelist[34];

  struct Output {
    uint8 palette[256];
    uint8 priority[256];
  } output;

  LayerWindow window;

  void frame();
  void update_list(uint64_t addr, uint8 data);
  void address_reset();
  void set_first();
  alwaysinline bool on_scanline(uint64_t sprite);
  void render();

  void serialize(serializer&);
  Sprite(PPU &self);

  PPU &self;
};
