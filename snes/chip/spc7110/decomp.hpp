class SPC7110Decomp {
public:
  uint8 read();
  void init(uint64_t mode, uint64_t offset, uint64_t index);
  void reset();

  void serialize(serializer&);
  SPC7110Decomp();
  ~SPC7110Decomp();

private:
  uint64_t decomp_mode;
  uint64_t decomp_offset;

  //read() will spool chunks half the size of decomp_buffer_size
  enum { decomp_buffer_size = 64 }; //must be >= 64, and must be a power of two
  uint8 *decomp_buffer;
  uint64_t decomp_buffer_rdoffset;
  uint64_t decomp_buffer_wroffset;
  uint64_t decomp_buffer_length;

  void write(uint8 data);
  uint8 dataread();

  void mode0(bool init);
  void mode1(bool init);
  void mode2(bool init);

  static const uint8 evolution_table[53][4];
  static const uint8 mode2_context_table[32][2];

  struct ContextState {
    uint8 index;
    uint8 invert;
  } context[32];

  uint8 probability(uint64_t n);
  uint8 next_lps(uint64_t n);
  uint8 next_mps(uint64_t n);
  bool toggle_invert(uint64_t n);

  uint64_t morton16[2][256];
  uint64_t morton32[4][256];
  uint64_t morton_2x8(uint64_t data);
  uint64_t morton_4x8(uint64_t data);
};
