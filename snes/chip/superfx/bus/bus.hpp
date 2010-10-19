struct SuperFXBus : Bus {
  void init();
};

struct SuperFXGSUROM : Memory {
  uint64_t size() const;
  uint8 read(uint64_t);
  void write(uint64_t, uint8);
};

struct SuperFXGSURAM : Memory {
  uint64_t size() const;
  uint8 read(uint64_t);
  void write(uint64_t, uint8);
};

struct SuperFXCPUROM : Memory {
  uint64_t size() const;
  uint8 read(uint64_t);
  void write(uint64_t, uint8);
};

struct SuperFXCPURAM : Memory {
  uint64_t size() const;
  uint8 read(uint64_t);
  void write(uint64_t, uint8);
};

namespace memory {
  extern SuperFXGSUROM gsurom;
  extern SuperFXGSURAM gsuram;
  extern SuperFXCPUROM fxrom;
  extern SuperFXCPURAM fxram;
}
