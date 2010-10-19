bool Cheat::active() const { return cheat_enabled; }
bool Cheat::exists(uint64_t addr) const { return bitmask[addr >> 3] & 1 << (addr & 7); }
