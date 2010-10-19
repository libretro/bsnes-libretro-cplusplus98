class DSP3 : public Memory {
public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read (uint64_t addr);
  void  write(uint64_t addr, uint8 data);
};

extern DSP3 dsp3;
