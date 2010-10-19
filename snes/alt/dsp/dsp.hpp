#include "SPC_DSP.h"

class DSP : public Processor, public ChipDebugger {
public:
  enum{ Threaded = false };
  alwaysinline void step(uint64_t clocks);
  alwaysinline void synchronize_smp();

  uint8 read(uint8 addr);
  void write(uint8 addr, uint8 data);

  void enter();
  void power();
  void reset();

  void channel_enable(uint64_t channel, bool enable);

  void serialize(serializer&);
  bool property(uint64_t id, string &name, string &value) { return false; }
  DSP();

private:
  SPC_DSP spc_dsp;
  int16 samplebuffer[8192];
  bool channel_enabled[8];
};

extern DSP dsp;
