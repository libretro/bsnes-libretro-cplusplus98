struct VBRBus : Bus {
  void init();
};

struct SA1Bus : Bus {
  void init();
};

struct VSPROM : Memory {
  uint64_t size() const;
  alwaysinline uint8 read(uint64_t);
  alwaysinline void write(uint64_t, uint8);
};

struct CPUIRAM : Memory {
  uint64_t size() const;
  alwaysinline uint8 read(uint64_t);
  alwaysinline void write(uint64_t, uint8);
};

struct SA1IRAM : Memory {
  uint64_t size() const;
  alwaysinline uint8 read(uint64_t);
  alwaysinline void write(uint64_t, uint8);
};

struct SA1BWRAM : Memory {
  uint64_t size() const;
  alwaysinline uint8 read(uint64_t);
  alwaysinline void write(uint64_t, uint8);
};

struct CC1BWRAM : Memory {
  uint64_t size() const;
  alwaysinline uint8 read(uint64_t);
  alwaysinline void write(uint64_t, uint8);
  bool dma;
};

struct BitmapRAM : Memory {
  uint64_t size() const;
  alwaysinline uint8 read(uint64_t);
  alwaysinline void write(uint64_t, uint8);
};

namespace memory {
  extern StaticRAM iram;

  extern VSPROM vsprom;
  extern CPUIRAM cpuiram;
  extern SA1IRAM sa1iram;
  extern SA1BWRAM sa1bwram;
  extern CC1BWRAM cc1bwram;
  extern BitmapRAM bitmapram;
};
