#ifndef __SNES_PPU_H
#define __SNES_PPU_H

class PPU : public Processor, public PPUcounter, public MMIO {
public:
  enum{ Threaded = true };
  alwaysinline void step(uint64_t clocks);
  alwaysinline void synchronize_cpu();

  void latch_counters();
  uint64_t interlace() const;
  uint64_t overscan() const;
  uint64_t hires() const;

  void enter();
  void power();
  void reset();
  void scanline();
  void frame();

  void layer_enable(uint64_t layer, uint64_t priority, uint64_t enable);
  void set_frameskip(uint64_t frameskip);

  void serialize(serializer&);
  PPU();
  ~PPU();

private:
  uint16 *surface;
  uint16 *output;


  #include "mmio/mmio.hpp"
  #include "window/window.hpp"
  #include "cache/cache.hpp"
  #include "background/background.hpp"
  #include "sprite/sprite.hpp"
  #include "screen/screen.hpp"

  friend class PPU::Cache;
  friend class PPU::Background;
  friend class PPU::Sprite;
  friend class PPU::Screen;
  friend class Video;

  Cache cache;
  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  Sprite oam;
  Screen screen;

  struct Display {
    uint64_t interlace;
    uint64_t overscan;
    uint64_t width;
    uint64_t height;
    uint64_t frameskip;
    uint64_t framecounter;
  } display;

  static void Enter();
  void add_clocks(uint64_t clocks);
  void render_scanline();

};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern PPUDebugger ppu;
#else
  extern PPU ppu;
#endif

#endif
