#ifdef CPU_CPP

uint8 CPU::pio() {
  return status.pio;
}

bool CPU::joylatch() {
  return 0;
}

bool CPU::interrupt_pending() {
  return false;
}

uint8 CPU::port_read(uint8 port) {
  return port_data[port & 3];
}

void CPU::port_write(uint8 port, uint8 data) {
  port_data[port & 3] = data;
}

void CPU::op_io() {
  add_clocks(6);
}

uint8 CPU::op_read(uint64_t addr) {
  regs.mdr = bus.read(addr);
  add_clocks(speed(addr));
  return regs.mdr;
}

void CPU::op_write(uint64_t addr, uint8 data) {
  add_clocks(speed(addr));
  bus.write(addr, regs.mdr = data);
}

uint64_t CPU::speed(uint64_t addr) const {
  if(addr & 0x408000) {
    if(addr & 0x800000) return status.rom_speed;
    return 8;
  }
  if((addr + 0x6000) & 0x4000) return 8;
  if((addr - 0x4000) & 0x7e00) return 6;
  return 12;
}

#endif
