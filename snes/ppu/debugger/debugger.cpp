#ifdef PPU_CPP

bool PPUDebugger::property(uint64_t id, string &name, string &value) {
  uint64_t n = 0;

  #define item(name_, value_) \
  if(id == n++) { \
    name = name_; \
    value = value_; \
    return true; \
  }

  #undef item
  return false;
}

#endif
