#ifdef PPU_CPP

#include "mode7.cpp"

void PPU::Background::frame() {
}

void PPU::Background::scanline() {
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);
  x = -7;
  y = self.vcounter();
  tile_counter = (7 - (regs.hoffset & 7)) << hires;
  for(uint64_t n = 0; n < 8; n++) data[n] = 0;

  if(self.vcounter() == 1) {
    mosaic_vcounter = regs.mosaic + 1;
    mosaic_voffset = 1;
  } else if(--mosaic_vcounter == 0) {
    mosaic_vcounter = regs.mosaic + 1;
    mosaic_voffset += regs.mosaic + 1;
  }

  mosaic_hcounter = regs.mosaic + 1;
  mosaic_hoffset = 0;
}

void PPU::Background::get_tile() {
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);

  uint64_t color_depth = (regs.mode == Mode::BPP2 ? 0 : regs.mode == Mode::BPP4 ? 1 : 2);
  uint64_t palette_offset = (self.regs.bgmode == 0 ? (id << 5) : 0);
  uint64_t palette_size = 2 << color_depth;
  uint64_t tile_mask = 0x0fff >> color_depth;
  uint64_t tiledata_index = regs.tiledata_addr >> (4 + color_depth);

  uint64_t tile_height = (regs.tile_size == TileSize::Size8x8 ? 3 : 4);
  uint64_t tile_width = (!hires ? tile_height : 4);

  uint64_t width = 256 << hires;

  uint64_t mask_x = (tile_height == 3 ? width : (width << 1));
  uint64_t mask_y = mask_x;
  if(regs.screen_size & 1) mask_x <<= 1;
  if(regs.screen_size & 2) mask_y <<= 1;
  mask_x--;
  mask_y--;

  uint64_t px = x << hires;
  uint64_t py = (regs.mosaic == 0 ? y : mosaic_voffset);

  uint64_t hscroll = regs.hoffset;
  uint64_t vscroll = regs.voffset;
  if(hires) {
    hscroll <<= 1;
    if(self.regs.interlace) py = (py << 1) + self.field();
  }

  uint64_t hoffset = hscroll + px;
  uint64_t voffset = vscroll + py;

  if(self.regs.bgmode == 2 || self.regs.bgmode == 4 || self.regs.bgmode == 6) {
    uint16 offset_x = (x + (hscroll & 7));

    if(offset_x >= 8) {
      uint64_t hval = self.bg3.get_tile((offset_x - 8) + (self.bg3.regs.hoffset & ~7), self.bg3.regs.voffset + 0);
      uint64_t vval = self.bg3.get_tile((offset_x - 8) + (self.bg3.regs.hoffset & ~7), self.bg3.regs.voffset + 8);
      uint64_t valid_mask = (id == ID::BG1 ? 0x2000 : 0x4000);

      if(self.regs.bgmode == 4) {
        if(hval & valid_mask) {
          if((hval & 0x8000) == 0) {
            hoffset = offset_x + (hval & ~7);
          } else {
            voffset = y + hval;
          }
        }
      } else {
        if(hval & valid_mask) hoffset = offset_x + (hval & ~7);
        if(vval & valid_mask) voffset = y + vval;
      }
    }
  }

  hoffset &= mask_x;
  voffset &= mask_y;

  uint64_t screen_x = (regs.screen_size & 1 ? 32 << 5 : 0);
  uint64_t screen_y = (regs.screen_size & 2 ? 32 << 5 : 0);
  if(regs.screen_size == 3) screen_y <<= 1;

  uint64_t tx = hoffset >> tile_width;
  uint64_t ty = voffset >> tile_height;

  uint16 offset = ((ty & 0x1f) << 5) + (tx & 0x1f);
  if(tx & 0x20) offset += screen_x;
  if(ty & 0x20) offset += screen_y;

  uint16 addr = regs.screen_addr + (offset << 1);
  tile = (memory::vram[addr + 0] << 0) + (memory::vram[addr + 1] << 8);
  bool mirror_y = tile & 0x8000;
  bool mirror_x = tile & 0x4000;
  priority = (tile & 0x2000 ? regs.priority1 : regs.priority0);
  palette_number = (tile >> 10) & 7;
  palette_index = palette_offset + (palette_number << palette_size);

  if(tile_width   == 4 && (bool)(hoffset & 8) != mirror_x) tile +=  1;
  if(tile_height  == 4 && (bool)(voffset & 8) != mirror_y) tile += 16;
  uint16 character = ((tile & 0x03ff) + tiledata_index) & tile_mask;

  if(mirror_y) voffset ^= 7;
  offset = (character << (4 + color_depth)) + ((voffset & 7) << 1);

  if(regs.mode >= Mode::BPP2) {
    data[0] = memory::vram[offset +  0];
    data[1] = memory::vram[offset +  1];
  }
  if(regs.mode >= Mode::BPP4) {
    data[2] = memory::vram[offset + 16];
    data[3] = memory::vram[offset + 17];
  }
  if(regs.mode >= Mode::BPP8) {
    data[4] = memory::vram[offset + 32];
    data[5] = memory::vram[offset + 33];
    data[6] = memory::vram[offset + 48];
    data[7] = memory::vram[offset + 49];
  }

  if(mirror_x) for(uint64_t n = 0; n < 8; n++) {
    //reverse data bits in data[n]: 01234567 -> 76543210
    data[n] = ((data[n] >> 4) & 0x0f) | ((data[n] << 4) & 0xf0);
    data[n] = ((data[n] >> 2) & 0x33) | ((data[n] << 2) & 0xcc);
    data[n] = ((data[n] >> 1) & 0x55) | ((data[n] << 1) & 0xaa);
  }
}

void PPU::Background::run(bool screen) {
  if(self.vcounter() == 0) return;
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);

  if(screen == Screen::Sub) {
    output.main.priority = 0;
    output.sub.priority = 0;
    if(hires == false) return;
  }

  if(regs.mode == Mode::Inactive) return;
  if(regs.main_enable == false && regs.sub_enable == false) return;

  if(regs.mode == Mode::Mode7) return run_mode7();

  if(tile_counter-- == 0) {
    tile_counter = 7;
    get_tile();
  }

  uint8 palette = get_tile_color();
  if(x == 0) mosaic_hcounter = 1;
  if(x >= 0 && --mosaic_hcounter == 0) {
    mosaic_hcounter = regs.mosaic + 1;
    mosaic_priority = priority;
    mosaic_palette = palette ? palette_index + palette : 0;
    mosaic_tile = tile;
  }
  if(screen == Screen::Main) x++;
  if(mosaic_palette == 0) return;

  if(hires == false) {
    if(regs.main_enable) {
      output.main.priority = mosaic_priority;
      output.main.palette = mosaic_palette;
      output.main.tile = mosaic_tile;
    }

    if(regs.sub_enable) {
      output.sub.priority = mosaic_priority;
      output.sub.palette = mosaic_palette;
      output.sub.tile = mosaic_tile;
    }
  } else if(screen == Screen::Main) {
    if(regs.main_enable) {
      output.main.priority = mosaic_priority;
      output.main.palette = mosaic_palette;
      output.main.tile = mosaic_tile;
    }
  } else if(screen == Screen::Sub) {
    if(regs.sub_enable) {
      output.sub.priority = mosaic_priority;
      output.sub.palette = mosaic_palette;
      output.sub.tile = mosaic_tile;
    }
  }
}

uint64_t PPU::Background::get_tile_color() {
  uint64_t color = 0;
  if(regs.mode >= Mode::BPP2) {
    color += (data[0] & 0x80) ? 0x01 : 0; data[0] <<= 1;
    color += (data[1] & 0x80) ? 0x02 : 0; data[1] <<= 1;
  }
  if(regs.mode >= Mode::BPP4) {
    color += (data[2] & 0x80) ? 0x04 : 0; data[2] <<= 1;
    color += (data[3] & 0x80) ? 0x08 : 0; data[3] <<= 1;
  }
  if(regs.mode >= Mode::BPP8) {
    color += (data[4] & 0x80) ? 0x10 : 0; data[4] <<= 1;
    color += (data[5] & 0x80) ? 0x20 : 0; data[5] <<= 1;
    color += (data[6] & 0x80) ? 0x40 : 0; data[6] <<= 1;
    color += (data[7] & 0x80) ? 0x80 : 0; data[7] <<= 1;
  }
  return color;
}

void PPU::Background::reset() {
  regs.tiledata_addr = 0;
  regs.screen_addr = 0;
  regs.screen_size = 0;
  regs.mosaic = 0;
  regs.tile_size = 0;
  regs.mode = 0;
  regs.priority0 = 0;
  regs.priority1 = 0;
  regs.main_enable = 0;
  regs.sub_enable = 0;
  regs.hoffset = 0;
  regs.voffset = 0;

  output.main.palette = 0;
  output.main.priority = 0;
  output.sub.palette = 0;
  output.sub.priority = 0;

  x = 0;
  y = 0;

  mosaic_vcounter = 0;
  mosaic_voffset = 0;
  mosaic_hcounter = 0;
  mosaic_hoffset = 0;

  mosaic_priority = 0;
  mosaic_palette = 0;
  mosaic_tile = 0;

  tile_counter = 0;
  tile = 0;
  priority = 0;
  palette_number = 0;
  palette_index = 0;
  for(uint64_t n = 0; n < 8; n++) data[n] = 0;
}

uint64_t PPU::Background::get_tile(uint64_t x, uint64_t y) {
  bool hires = (self.regs.bgmode == 5 || self.regs.bgmode == 6);
  uint64_t tile_height = (regs.tile_size == TileSize::Size8x8 ? 3 : 4);
  uint64_t tile_width = (!hires ? tile_height : 4);
  uint64_t width = (!hires ? 256 : 512);
  uint64_t mask_x = (tile_height == 3 ? width : (width << 1));
  uint64_t mask_y = mask_x;
  if(regs.screen_size & 1) mask_x <<= 1;
  if(regs.screen_size & 2) mask_y <<= 1;
  mask_x--;
  mask_y--;

  uint64_t screen_x = (regs.screen_size & 1 ? (32 << 5) : 0);
  uint64_t screen_y = (regs.screen_size & 2 ? (32 << 5) : 0);
  if(regs.screen_size == 3) screen_y <<= 1;

  x = (x & mask_x) >> tile_width;
  y = (y & mask_y) >> tile_height;

  uint16 offset = ((y & 0x1f) << 5) + (x & 0x1f);
  if(x & 0x20) offset += screen_x;
  if(y & 0x20) offset += screen_y;

  uint16 addr = regs.screen_addr + (offset << 1);
  return (memory::vram[addr + 0] << 0) + (memory::vram[addr + 1] << 8);
}

PPU::Background::Background(PPU &self, uint64_t id) : self(self), id(id) {
}

#endif
