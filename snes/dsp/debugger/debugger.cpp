#ifdef DSP_CPP

bool DSPDebugger::property(uint64_t id, string &name, string &value) {
  uint64_t n = 0;

  #define item(name_, value_) \
  if(id == n++) { \
    name = name_; \
    value = value_; \
    return true; \
  }

  item("Main Volume - Left", (uint64_t)state.regs[0x0c]);
  item("Main Volume - Right", (uint64_t)state.regs[0x1c]);
  item("Echo Volume - Left", (uint64_t)state.regs[0x2c]);
  item("Echo Volume - Right", (uint64_t)state.regs[0x3c]);
  item("Key On", string("0x", strhex<2>(state.regs[0x4c])));
  item("Key Off", string("0x", strhex<2>(state.regs[0x5c])));
  item("Flag - Reset", (bool)(state.regs[0x6c] & 0x80));
  item("Flag - Mute", (bool)(state.regs[0x6c] & 0x40));
  item("Flag - Echo Disable", (bool)(state.regs[0x6c] & 0x20));
  item("Flag - Noise Clock", (uint64_t)state.regs[0x6c] & 0x1f);
  item("Source End Block", (uint64_t)state.regs[0x7c]);
  item("Echo Feedback", (uint64_t)state.regs[0x0d]);
  item("Pitch Modulation Enable", string("0x", strhex<2>(state.regs[0x2d])));
  item("Noise Enable", string("0x", strhex<2>(state.regs[0x3d])));
  item("Echo Enable", string("0x", strhex<2>(state.regs[0x4d])));
  item("Source Directory", (uint64_t)state.regs[0x5d]);
  item("Echo Start Address", (uint64_t)state.regs[0x6d]);
  item("Echo Directory", (uint64_t)state.regs[0x7d]);

  for(uint64_t i = 0; i < 8; i++) {
    item(string("Coefficient ", i), string("0x", strhex<2>(state.regs[(i << 4) + 0x0f])));
  }

  for(uint64_t i = 0; i < 8; i++) {
    item(string("Voice ", i), "");
    item("Volume - Left", (uint64_t)state.regs[(i << 4) + 0x00]);
    item("Volume - Right", (uint64_t)state.regs[(i << 4) + 0x01]);
    item("Pitch Height", string("0x", strhex<4>(state.regs[(i << 4) + 0x02] + (state.regs[(i << 4) + 0x03] << 8))));
    item("Source Number", (uint64_t)state.regs[(i << 4) + 0x04]);
    item("ADSR1", (uint64_t)state.regs[(i << 4) + 0x05]);
    item("ADSR2", (uint64_t)state.regs[(i << 4) + 0x06]);
    item("GAIN", (uint64_t)state.regs[(i << 4) + 0x07]);
    item("ENVX", (uint64_t)state.regs[(i << 4) + 0x08]);
    item("OUTX", (uint64_t)state.regs[(i << 4) + 0x09]);
  }

  #undef item
  return false;
}

#endif
