#ifdef PPU_CPP

#include <ppu_intrinsics.h>

unsigned PPU::Screen::get_palette(const unsigned color) const {
#if 0
  #if defined(ARCH_LSB)
  static uint16 *cgram = (uint16*)memory::cgram.data();
  return cgram[color];
  #else
  color <<= 1;
  return (memory::cgram[color + 0] << 0) + (memory::cgram[color + 1] << 8);
  #endif
#endif
  // BIG ENDIAN!!
  return __lhbrx(cgram + (color << 1));
}

unsigned PPU::Screen::get_direct_color(const unsigned p, const unsigned t) const {
  return ((t & 7) << 2) | ((p & 1) << 1) |
         (((t >> 3) & 7) << 7) | (((p >> 1) & 1) << 6) |
         ((t >> 6) << 13) | ((p >> 2) << 12);
}

uint16 PPU::Screen::addsub(const unsigned x, const unsigned y, bool halve) const {
  if(!regs.color_mode) {
    if(!halve) {
      unsigned sum = x + y;
      unsigned carry = (sum - ((x ^ y) & 0x0421)) & 0x8420;
      return (sum - carry) | (carry - (carry >> 5));
    } else {
      return (x + y - ((x ^ y) & 0x0421)) >> 1;
    }
  } else {
    unsigned diff = x - y + 0x8420;
    unsigned borrow = (diff - ((x ^ y) & 0x8420)) & 0x8420;
    if(!halve) {
      return (diff - borrow) & (borrow - (borrow >> 5));
    } else {
      return (((diff - borrow) & (borrow - (borrow >> 5))) & 0x7bde) >> 1;
    }
  }
}

void PPU::Screen::scanline() {
  unsigned main_color = get_palette(0);
  unsigned sub_color = (self.regs.pseudo_hires == false && self.regs.bgmode != 5 && self.regs.bgmode != 6)
                     ? regs.color : main_color;

  for(unsigned x = 0; x < 256; x++) {
    output.main[x].color = main_color;
    output.main[x].priority = 0;
    output.main[x].source = 6;

    output.sub[x].color = sub_color;
    output.sub[x].priority = 0;
    output.sub[x].source = 6;
  }

  window.render(0);
  window.render(1);
}

void PPU::Screen::render_black() {
  uint16 *data = self.output + self.vcounter() * 1024;
  if(self.interlace() && self.field()) data += 512;
  memset(data, 0, self.display.width << 1);
}

uint16 PPU::Screen::get_pixel_main(const unsigned x) {
  typeof(output.main[0]) main = output.main[x];
  typeof(output.sub[0]) sub = output.sub[x];

  if(!regs.addsub_mode) {
    sub.source = 6;
    sub.color = regs.color;
  }

  if(!window.main[x]) {
    if(!window.sub[x]) {
      return 0x0000;
    }
    main.color = 0x0000;
  }

  if(main.source != 5 && regs.color_enable[main.source] && window.sub[x]) {
    bool halve = false;
    if(regs.color_halve && window.main[x]) {
      if(!regs.addsub_mode || sub.source != 6) halve = true;
    }
    return addsub(main.color, sub.color, halve);
  }

  return main.color;
}

uint16 PPU::Screen::get_pixel_sub(const unsigned x) {
  typeof(output.sub[0]) main = output.sub[x];
  typeof(output.main[0]) sub = output.main[x];

  if(!regs.addsub_mode) {
    sub.source = 6;
    sub.color = regs.color;
  }

  if(!window.main[x]) {
    if(!window.sub[x]) {
      return 0x0000;
    }
    main.color = 0x0000;
  }

  if(main.source != 5 && regs.color_enable[main.source] && window.sub[x]) {
    bool halve = false;
    if(regs.color_halve && window.main[x]) {
      if(!regs.addsub_mode || sub.source != 6) halve = true;
    }
    return addsub(main.color, sub.color, halve);
  }

  return main.color;
}

void PPU::Screen::render() {
  uint16 *data = self.output + self.vcounter() * 1024;
  if(self.interlace() && self.field()) data += 512;
  uint16 *light = light_table[self.regs.display_brightness];

  if(!self.regs.pseudo_hires && self.regs.bgmode != 5 && self.regs.bgmode != 6) {
    for(uint64_t i = 0; i < 256; i++) {
      data[i] = light[get_pixel_main(i)];
    }
  } else {
    for(uint64_t i = 0; i < 256; i++) {
      *data++ = light[get_pixel_sub(i)];
      *data++ = light[get_pixel_main(i)];
    }
  }
}

PPU::Screen::Screen(PPU &self) : self(self), cgram(memory::cgram.data()) {
  light_table = (uint16_t**)memalign(128, 16 * sizeof(uint16_t*));
  for(uint64_t l = 0; l < 16; l++) {
    light_table[l] = (uint16_t*)memalign(128, 32768 * sizeof(uint16_t));
    for(uint64_t r = 0; r < 32; r++) {
      for(uint64_t g = 0; g < 32; g++) {
        for(uint64_t b = 0; b < 32; b++) {
          double luma = (double)l / 15.0;
          uint64_t ar = static_cast<unsigned>(luma * r + 0.5);
          uint64_t ag = static_cast<unsigned>(luma * g + 0.5);
          uint64_t ab = static_cast<unsigned>(luma * b + 0.5);
          light_table[l][(r << 10) + (g << 5) + (b << 0)] = (ab << 10) + (ag << 5) + (ar << 0);
        }
      }
    }
  }
}

PPU::Screen::~Screen() {
  for(uint64_t l = 0; l < 16; l++) free(light_table[l]);
  free(light_table);
}

void PPU::Screen::Output::plot_main(const unsigned x, const unsigned color, const unsigned priority, const unsigned source) {
  if(priority > main[x].priority) {
    main[x].color = color;
    main[x].priority = priority;
    main[x].source = source;
  }
}

void PPU::Screen::Output::plot_sub(const unsigned x, const unsigned color, const unsigned priority, const unsigned source) {
  if(priority > sub[x].priority) {
    sub[x].color = color;
    sub[x].priority = priority;
    sub[x].source = source;
  }
}

#endif
