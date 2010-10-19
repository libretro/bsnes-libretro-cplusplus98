class Background {
  struct ID { enum { BG1, BG2, BG3, BG4 }; };
  struct Mode { enum { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenSize { enum { Size32x32, Size32x64, Size64x32, Size64x64 }; };
  struct TileSize { enum { Size8x8, Size16x16 }; };

  bool priority0_enable;
  bool priority1_enable;

  struct Regs {
    uint64_t mode;
    uint64_t priority0;
    uint64_t priority1;

    bool tile_size;
    uint64_t mosaic;

    uint64_t screen_addr;
    uint64_t screen_size;
    uint64_t tiledata_addr;

    uint64_t hoffset;
    uint64_t voffset;

    bool main_enable;
    bool sub_enable;
  } regs;

  uint16 **mosaic_table;

  const uint64_t id;
  uint64_t opt_valid_bit;

  uint64_t y;
  bool hires;
  signed width;

  uint64_t tile_width;
  uint64_t tile_height;

  uint64_t mask_x;
  uint64_t mask_y;

  uint64_t scx;
  uint64_t scy;

  uint64_t hscroll;
  uint64_t vscroll;

  uint64_t mosaic_vcounter;

  LayerWindow window;

  alwaysinline uint64_t get_tile(uint64_t hoffset, uint64_t voffset);
  void offset_per_tile(uint64_t x, uint64_t y, uint64_t &hoffset, uint64_t &voffset);
  void scanline();
  void render();
  void render_mode7();

  void serialize(serializer&);
  Background(PPU &self, uint64_t id);
  ~Background();

  PPU &self;
  friend class PPU;
};
