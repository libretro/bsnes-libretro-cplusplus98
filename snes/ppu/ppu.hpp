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

  void serialize(serializer&);
  PPU();
  ~PPU();

private:
  uint16 *surface;
  uint16 *output;

  uint8 ppu1_version;
  uint8 ppu2_version;

  struct {
    uint64_t interlace;
    uint64_t overscan;
  } display;

  #include "background/background.hpp"
  #include "mmio/mmio.hpp"
  #include "screen/screen.hpp"
  #include "sprite/sprite.hpp"
  #include "window/window.hpp"

  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  Sprite oam;
  Window window;
  Screen screen;

  static void Enter();
  void add_clocks(uint64_t);

  void scanline();
  void frame();

  friend class PPU::Background;
  friend class PPU::Sprite;
  friend class PPU::Window;
  friend class PPU::Screen;
  friend class Video;
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern PPUDebugger ppu;
#else
  extern PPU ppu;
#endif
