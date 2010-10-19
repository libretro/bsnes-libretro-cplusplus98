#ifdef SYSTEM_CPP

Video video;

const uint8_t Video::cursor[15 * 15] = {
  0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
  0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,
  0,0,0,1,2,2,1,2,1,2,2,1,0,0,0,
  0,0,1,2,1,1,0,1,0,1,1,2,1,0,0,
  0,1,2,1,0,0,0,1,0,0,0,1,2,1,0,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,
  1,2,1,0,0,1,1,2,1,1,0,0,1,2,1,
  1,2,2,1,1,2,2,2,2,2,1,1,2,2,1,
  1,2,1,0,0,1,1,2,1,1,0,0,1,2,1,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,
  0,1,2,1,0,0,0,1,0,0,0,1,2,1,0,
  0,0,1,2,1,1,0,1,0,1,1,2,1,0,0,
  0,0,0,1,2,2,1,2,1,2,2,1,0,0,0,
  0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,
  0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
};

void Video::draw_cursor(uint16_t color, int x, int y) {
  uint16_t *data = (uint16_t*)ppu.output;
  if(ppu.interlace() && ppu.field()) data += 512;

  for(uint64_t cy = 0; cy < 15; cy++) {
    uint64_t vy = y + cy - 7;
    if(vy <= 0 || vy >= 240) continue;  //do not draw offscreen

    uint64_t hires = (line_width[vy] == 512);
    for(uint64_t cx = 0; cx < 15; cx++) {
      int vx = x + cx - 7;
      if(vx < 0 || vx >= 256) continue;  //do not draw offscreen
      uint64_t pixel = cursor[cy * 15 + cx];
      if(pixel == 0) continue;
      uint16_t pixelcolor = (pixel == 1) ? 0 : color;

      if(!hires) {
        *((uint16_t*)data + vy * 1024 + vx) = pixelcolor;
      } else {
        *((uint16_t*)data + vy * 1024 + vx * 2 + 0) = pixelcolor;
        *((uint16_t*)data + vy * 1024 + vx * 2 + 1) = pixelcolor;
      }
    }
  }
}

void Video::update() {
  switch(input.port[1].device.i) {
    case Input::Device::SuperScope: draw_cursor(0x001f, input.port[1].superscope.x, input.port[1].superscope.y); break;
    case Input::Device::Justifiers: draw_cursor(0x02e0, input.port[1].justifier.x2, input.port[1].justifier.y2); //fallthrough
    case Input::Device::Justifier:  draw_cursor(0x001f, input.port[1].justifier.x1, input.port[1].justifier.y1); break;
  }

  uint16_t *data = (uint16_t*)ppu.output;
  if(ppu.interlace() && ppu.field()) data += 512;
  uint64_t width = 256;
  uint64_t height = !ppu.overscan() ? 224 : 239;

  if(frame_hires) {
    width <<= 1;
    //normalize line widths
    for(uint64_t y = 0; y < 240; y++) {
      if(line_width[y] == 512) continue;
      uint16_t *buffer = data + y * 1024; // Bug?
      for(uint64_t x = 255; x >= 0; x--) {
        buffer[(x * 2) + 0] = buffer[(x * 2) + 1] = buffer[x];
      }
    }
  }

  height <<= frame_interlace;

  system.interface->video_refresh(ppu.output + 1024, width, height);

  frame_hires = 0;
  frame_interlace = 0;
}

void Video::scanline() {
  uint64_t y = cpu.vcounter();
  if(y >= 240) return;

  frame_hires |= ppu.hires();
  frame_interlace |= ppu.interlace();
  uint64_t width = (!ppu.hires() ? 256 : 512);
  line_width[y] = width;
}

void Video::init() {
  frame_hires = 0;
  frame_interlace = 0;
  for(uint64_t i = 0; i < 240; i++) line_width[i] = 256;
}

#endif
