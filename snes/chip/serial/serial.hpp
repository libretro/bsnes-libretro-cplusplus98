class Serial : public Coprocessor, public MMIO, public library, public property<Serial> {
public:
  static void Enter();
  void enter();
  void init();
  void enable();
  void power();
  void reset();
  void serialize(serializer&);

  readonly<bool> data1;
  readonly<bool> data2;

  void add_clocks(uint64_t clocks);
  uint8 read();
  void write(uint8 data);

  uint8 mmio_read(uint64_t addr);
  void mmio_write(uint64_t addr, uint8 data);

private:
  MMIO *r4016, *r4017;
  function<uint64_t ()> baudrate;
  function<bool ()> flowcontrol;
  function<void (void (*)(uint64_t), uint8_t (*)(), void (*)(uint8_t))> main;
};

extern Serial serial;
