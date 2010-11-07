struct Screen {
  struct Regs {
    bool addsub_mode;
    bool direct_color;

    bool color_mode;
    bool color_halve;
    bool color_enable[7];

    unsigned color_b;
    unsigned color_g;
    unsigned color_r;
    unsigned color;
  } regs;

  struct Output {
    struct Pixel {
      unsigned color;
      unsigned priority;
      unsigned source;
    } main[256], sub[256];

    alwaysinline void plot_main(unsigned x, unsigned color, unsigned priority, unsigned source);
    alwaysinline void plot_sub(unsigned x, unsigned color, unsigned priority, unsigned source);
  } output;

  ColorWindow window;
  uint16 **light_table;
  uint8 *cgram;

  alwaysinline unsigned get_palette(const unsigned color) const;
  unsigned get_direct_color(const unsigned palette, const unsigned tile) const;
  alwaysinline uint16 addsub(const unsigned x, const unsigned y, const bool halve) const;
  void scanline();
  void render_black();
  alwaysinline uint16 get_pixel_main(const unsigned x);
  alwaysinline uint16 get_pixel_sub(const unsigned x);
  void render();

  void serialize(serializer&);
  Screen(PPU &self);
  ~Screen();

  PPU &self;
};
