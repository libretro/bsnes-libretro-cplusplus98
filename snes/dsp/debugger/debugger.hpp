class DSPDebugger : public DSP, public ChipDebugger {
public:
  bool property(uint64_t id, string &name, string &value);
};
