struct CheatCode {
  bool enabled;
  array<uint64_t> addr;
  array<uint8> data;

  bool operator=(string);
  CheatCode();
};

class Cheat : public linear_vector<CheatCode> {
public:
  struct Type{ enum e{ ProActionReplay, GameGenie } i; };

  bool enabled() const;
  void enable(bool);
  void synchronize();
  bool read(uint64_t, uint8&) const;

  inline bool active() const;
  inline bool exists(uint64_t addr) const;

  Cheat();

  static bool decode(const char*, uint64_t&, uint8&, Type&);
  static bool encode(string&, uint64_t, uint8, Type);

private:
  uint8 bitmask[0x200000];
  bool system_enabled;
  bool code_enabled;
  bool cheat_enabled;
  uint64_t mirror(uint64_t) const;
};

extern Cheat cheat;
