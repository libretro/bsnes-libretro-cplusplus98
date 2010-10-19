struct Screen {
  struct Regs {
    bool addsub_mode;
    bool direct_color;

    bool color_mode;
    bool color_halve;
    bool color_enable[7];

    uint64_t color_b;
    uint64_t color_g;
    uint64_t color_r;
    uint64_t color;
  } regs;

  struct Output {
    struct Pixel {
      uint64_t color;
      uint64_t priority;
      uint64_t source;
    } main[256], sub[256];

    alwaysinline void plot_main(uint64_t x, uint64_t color, uint64_t priority, uint64_t source);
    alwaysinline void plot_sub(uint64_t x, uint64_t color, uint64_t priority, uint64_t source);
  } output;

  ColorWindow window;
  uint16 **light_table;

  alwaysinline uint64_t get_palette(uint64_t color);
  uint64_t get_direct_color(uint64_t palette, uint64_t tile);
  alwaysinline uint16 addsub(uint64_t x, uint64_t y, bool halve);
  void scanline();
  void render_black();
  alwaysinline uint16 get_pixel_main(uint64_t x);
  alwaysinline uint16 get_pixel_sub(uint64_t x);
  void render();

  void serialize(serializer&);
  Screen(PPU &self);
  ~Screen();

  PPU &self;
};
