#include <snes/snes.hpp>

#define CONTROLLER_CPP
namespace SNES {

#include "gamepad/gamepad.cpp"
#include "multitap/multitap.cpp"
#include "mouse/mouse.cpp"
#include "superscope/superscope.cpp"
#include "justifier/justifier.cpp"

void Controller::Enter() {
  if(co_active() == input.port1->thread) input.port1->enter();
  if(co_active() == input.port2->thread) input.port2->enter();
}

void Controller::enter() {
  while(true) step(1);
}

void Controller::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
  synchronize_cpu();
}

void Controller::synchronize_cpu() {
  if(CPU::Threaded == true) {
    if(clock >= 0 && scheduler.sync.i != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
  } else {
    while(clock >= 0) cpu.enter();
  }
}

bool Controller::iobit() {
  switch(port ? 1 : 0) {
  case Controller::Port1: return cpu.pio() & 0x40;
  case Controller::Port2: return cpu.pio() & 0x80;
  }

  return false;
}

void Controller::iobit(bool data) {
  switch(port ? 1 : 0) {
  case Controller::Port1: bus.write(0x4201, (cpu.pio() & ~0x40) | (data << 6)); break;
  case Controller::Port2: bus.write(0x4201, (cpu.pio() & ~0x80) | (data << 7)); break;
  }
}

Controller::Controller(bool port) : port(port) {
  if(!thread) create(Controller::Enter, 1);
}

}
