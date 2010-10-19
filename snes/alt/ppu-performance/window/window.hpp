struct LayerWindow {
  bool one_enable;
  bool one_invert;
  bool two_enable;
  bool two_invert;

  uint64_t mask;

  bool main_enable;
  bool sub_enable;

  uint8 main[256];
  uint8 sub[256];

  void render(bool screen);
  void serialize(serializer&);
};

struct ColorWindow {
  bool one_enable;
  bool one_invert;
  bool two_enable;
  bool two_invert;

  uint64_t mask;

  uint64_t main_mask;
  uint64_t sub_mask;

  uint8 main[256];
  uint8 sub[256];

  void render(bool screen);
  void serialize(serializer&);
};
