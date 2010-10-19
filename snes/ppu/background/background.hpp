class Background {
  struct ID { enum { BG1, BG2, BG3, BG4 }; };
  uint64_t id;

  struct Mode { enum  { BPP2, BPP4, BPP8, Mode7, Inactive }; };
  struct ScreenSize { enum { Size32x32, Size32x64, Size64x32, Size64x64 }; };
  struct TileSize { enum { Size8x8, Size16x16 }; };
  struct Screen { enum { Main, Sub }; };

  struct Regs {
    uint64_t tiledata_addr;
    uint64_t screen_addr;
    uint64_t screen_size;
    uint64_t mosaic;
    bool tile_size;

    uint64_t mode;
    uint64_t priority0;
    uint64_t priority1;

    bool main_enable;
    bool sub_enable;

    uint64_t hoffset;
    uint64_t voffset;
  } regs;

  struct Output {
    struct Pixel {
      uint64_t priority;  //0 = none (transparent)
      uint8 palette;
      uint16 tile;
    } main, sub;
  } output;

  struct {
    signed x;
    signed y;

    uint64_t mosaic_vcounter;
    uint64_t mosaic_voffset;
    uint64_t mosaic_hcounter;
    uint64_t mosaic_hoffset;

    uint64_t mosaic_priority;
    uint8 mosaic_palette;
    uint16 mosaic_tile;

    uint64_t tile_counter;
    uint64_t tile;
    uint64_t priority;
    uint64_t palette_number;
    uint64_t palette_index;
    uint8 data[8];
  };

  void frame();
  void scanline();
  void run(bool screen);
  void reset();

  void get_tile();
  uint64_t get_tile_color();
  uint64_t get_tile(uint64_t x, uint64_t y);
  signed clip(signed n);
  void run_mode7();

  void serialize(serializer&);
  Background(PPU &self, uint64_t id);

  PPU &self;
  friend class PPU;
};
