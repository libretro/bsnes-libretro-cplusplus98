void op_io();
debugvirtual uint8 op_read(uint32 addr);
debugvirtual void op_write(uint32 addr, uint8 data);
alwaysinline uint64_t speed(uint64_t addr) const;
