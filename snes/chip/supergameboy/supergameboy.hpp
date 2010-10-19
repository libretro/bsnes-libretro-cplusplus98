class SuperGameBoy : public Coprocessor, public MMIO, public Memory, public library {
public:
  static void Enter();
  void enter();
  void save();

  MMIO *mmio[3];
  uint8 mmio_read(uint64_t addr);
  void mmio_write(uint64_t addr, uint8 data);

  uint8 read(uint64_t addr);
  void write(uint64_t addr, uint8 data);

  void init();
  void enable();
  void power();
  void reset();
  void unload();

  void serialize(serializer&);

private:
  uint32_t samplebuffer[4096];
  uint64_t row;

  function<void (uint8_t*, uint64_t)> sgb_rom;
  function<void (uint8_t*, uint64_t)> sgb_ram;
  function<void (uint8_t*, uint64_t)> sgb_rtc;
  function<bool (bool)> sgb_init;
  function<void ()> sgb_term;
  function<void ()> sgb_power;
  function<void ()> sgb_reset;
  function<void (uint64_t)> sgb_row;
  function<uint8 (uint16)> sgb_read;
  function<void (uint16, uint8)> sgb_write;
  function<uint64_t (uint32_t*, uint64_t)> sgb_run;
  function<void ()> sgb_save;
  function<void (serializer&)> sgb_serialize;

  friend class Cartridge;
};

extern SuperGameBoy supergameboy;
