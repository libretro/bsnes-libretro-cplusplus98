struct Cache {
  uint8 *tiledata[3];
  uint8 *tilevalid[3];

  uint8* tile_2bpp(uint64_t tile);
  uint8* tile_4bpp(uint64_t tile);
  uint8* tile_8bpp(uint64_t tile);
  uint8* tile(uint64_t bpp, uint64_t tile);

  void serialize(serializer&);
  Cache(PPU &self);

  PPU &self;
};
