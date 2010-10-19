class PPUDebugger : public PPU, public ChipDebugger {
public:
  bool property(uint64_t id, string &name, string &value);
};
