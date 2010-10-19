struct Memory {
  virtual inline uint64_t size() const;
  virtual uint8 read(uint64_t addr) = 0;
  virtual void write(uint64_t addr, uint8 data) = 0;
};

struct MMIO {
  virtual uint8 mmio_read(uint64_t addr) = 0;
  virtual void mmio_write(uint64_t addr, uint8 data) = 0;
};

struct UnmappedMemory : Memory {
  uint64_t size() const;
  uint8 read(uint64_t);
  void write(uint64_t, uint8);
};

struct UnmappedMMIO : MMIO {
  uint8 mmio_read(uint64_t);
  void mmio_write(uint64_t, uint8);
};

struct StaticRAM : Memory {
  inline uint8* data();
  inline uint64_t size() const;

  inline uint8 read(uint64_t addr);
  inline void write(uint64_t addr, uint8 n);
  inline uint8& operator[](uint64_t addr);
  inline const uint8& operator[](uint64_t addr) const;

  inline StaticRAM(uint64_t size);
  inline ~StaticRAM();

private:
  uint8 *data_;
  uint64_t size_;
};

struct MappedRAM : Memory {
  inline void reset();
  inline void map(uint8*, uint64_t);
  inline void copy(const uint8*, uint64_t);

  inline void write_protect(bool status);
  inline uint8* data();
  inline uint64_t size() const;

  inline uint8 read(uint64_t addr);
  inline void write(uint64_t addr, uint8 n);
  inline const uint8& operator[](uint64_t addr) const;
  inline MappedRAM();

private:
  uint8 *data_;
  uint64_t size_;
  bool write_protect_;
};

struct MMIOAccess : Memory {
  MMIO* handle(uint64_t addr);
  void map(uint64_t addr, MMIO &access);
  uint8 read(uint64_t addr);
  void write(uint64_t addr, uint8 data);
  MMIOAccess();

private:
  MMIO *mmio[0x8000];
};

struct Bus {
  uint64_t mirror(uint64_t addr, uint64_t size);
  void map(uint64_t addr, Memory &access, uint64_t offset);
  struct MapMode{ enum e{ Direct, Linear, Shadow } i; };
  void map(MapMode::e mode,
    uint8  bank_lo, uint8  bank_hi,
    uint16 addr_lo, uint16 addr_hi,
    Memory &access, uint64_t offset = 0, uint64_t size = 0);

  alwaysinline uint8 read(uint24 addr);
  alwaysinline void write(uint24 addr, uint8 data);

  bool load_cart();
  void unload_cart();

  void power();
  void reset();

  struct Page {
    Memory *access;
    uint64_t offset;
  } page[65536];

  void serialize(serializer&);

private:
  void map_reset();
  void map_xml();
  void map_system();
};

namespace memory {
  extern MMIOAccess mmio;   //S-CPU, S-PPU
  extern StaticRAM wram;    //S-CPU
  extern StaticRAM apuram;  //S-SMP, S-DSP
  extern StaticRAM vram;    //S-PPU
  extern StaticRAM oam;     //S-PPU
  extern StaticRAM cgram;   //S-PPU

  extern UnmappedMemory memory_unmapped;
  extern UnmappedMMIO mmio_unmapped;
};

extern Bus bus;
