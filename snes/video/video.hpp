class Video {
private:
  uint64_t frame_hires;
  uint64_t frame_interlace;
  uint64_t line_width[240];

  void update();
  void scanline();
  void init();

  static const uint8_t cursor[15 * 15];
  void draw_cursor(uint16_t color, int x, int y);

  friend class System;
};

extern Video video;
