class Sprite {
  struct SpriteItem {
    uint16 x;
    uint16 y;
    uint8 character;
    bool nameselect;
    bool vflip;
    bool hflip;
    uint8 priority;
    uint8 palette;
    bool size;
    uint64_t width() const;
    uint64_t height() const;
  } list[128];

  struct TileItem {
    uint16 x;
    uint16 priority;
    uint16 palette;
    bool hflip;
    uint8 d0, d1, d2, d3;
  };

  struct State {
    uint64_t x;
    uint64_t y;

    uint64_t item_count;
    uint64_t tile_count;

    bool active;
    uint8 item[2][32];
    TileItem tile[2][34];
  } t;

  struct Regs {
    bool main_enable;
    bool sub_enable;
    bool interlace;

    uint8 base_size;
    uint8 nameselect;
    uint16 tiledata_addr;
    uint8 first_sprite;

    uint64_t priority0;
    uint64_t priority1;
    uint64_t priority2;
    uint64_t priority3;

    bool time_over;
    bool range_over;
  } regs;

  struct Output {
    struct Pixel {
      uint64_t priority;  //0 = none (transparent)
      uint8 palette;
    } main, sub;
  } output;

  //list.cpp
  void update(uint64_t addr, uint8 data);

  //sprite.cpp
  void address_reset();
  void set_first_sprite();
  void frame();
  void scanline();
  void run();
  void tilefetch();
  void reset();

  bool on_scanline(SpriteItem&);

  void serialize(serializer&);
  Sprite(PPU &self);

  PPU &self;
  friend class PPU;
};
