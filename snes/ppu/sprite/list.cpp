#ifdef PPU_CPP

void PPU::Sprite::update(uint64_t addr, uint8 data) {
  memory::oam[addr] = data;

  if(addr < 0x0200) {
    uint64_t n = addr >> 2;
    addr &= 3;
    if(addr == 0) {
      list[n].x = (list[n].x & 0x100) | data;
    } else if(addr == 1) {
      list[n].y = data;
    } else if(addr == 2) {
      list[n].character = data;
    } else {  //(addr == 3)
      list[n].vflip = data & 0x80;
      list[n].hflip = data & 0x40;
      list[n].priority = (data >> 4) & 3;
      list[n].palette = (data >> 1) & 7;
      list[n].nameselect = data & 1;
    }
  } else {
    uint64_t n = (addr & 0x1f) << 2;
    list[n + 0].x = ((data & 0x01) << 8) | (list[n + 0].x & 0xff);
    list[n + 0].size = data & 0x02;
    list[n + 1].x = ((data & 0x04) << 6) | (list[n + 1].x & 0xff);
    list[n + 1].size = data & 0x08;
    list[n + 2].x = ((data & 0x10) << 4) | (list[n + 2].x & 0xff);
    list[n + 2].size = data & 0x20;
    list[n + 3].x = ((data & 0x40) << 2) | (list[n + 3].x & 0xff);
    list[n + 3].size = data & 0x80;
  }
}

uint64_t PPU::Sprite::SpriteItem::width() const {
  if(size == 0) {
    static uint64_t width[] = {  8,  8,  8, 16, 16, 32, 16, 16 };
    return width[ppu.oam.regs.base_size];
  } else {
    static uint64_t width[] = { 16, 32, 64, 32, 64, 64, 32, 32 };
    return width[ppu.oam.regs.base_size];
  }
}

uint64_t PPU::Sprite::SpriteItem::height() const {
  if(size == 0) {
    if(ppu.oam.regs.interlace && ppu.oam.regs.base_size >= 6) return 16;
    static uint64_t height[] = {  8,  8,  8, 16, 16, 32, 32, 32 };
    return height[ppu.oam.regs.base_size];
  } else {
    static uint64_t height[] = { 16, 32, 64, 32, 64, 64, 64, 32 };
    return height[ppu.oam.regs.base_size];
  }
}

#endif
