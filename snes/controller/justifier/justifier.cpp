#ifdef CONTROLLER_CPP

void Justifier::enter() {
  unsigned prev = 0;
  while(true) {
    unsigned next = cpu.vcounter() * 1364 + cpu.hcounter();

    signed x = (active == 0 ? x1 : x2), y = (active == 0 ? y1 : y2);
    bool offscreen = (x < 0 || y < 0 || x >= 256 || y >= (ppu.overscan() ? 240 : 225));

    if(offscreen == false) {
      unsigned target = y * 1364 + (x + 24) * 4;
      if(next >= target && prev < target) {
        //CRT raster detected, toggle iobit to latch counters
        iobit(0);
        iobit(1);
      }
    }

    if(next < prev) {
      int nx1 = interface->inputPoll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::X);
      int ny1 = interface->inputPoll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::Y);
      nx1 += x1;
      ny1 += y1;
      x1 = max(-16, min(256 + 16, nx1));
      y1 = max(-16, min(240 + 16, ny1));

      if(chained == true) {
        int nx2 = interface->inputPoll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::X);
        int ny2 = interface->inputPoll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::Y);
        nx2 += x2;
        ny2 += y2;
        x2 = max(-16, min(256 + 16, nx2));
        y2 = max(-16, min(240 + 16, ny2));
      }
    } else {
      //sleep until PPU counters are close to latch position
      unsigned diff = abs((signed)y - cpu.vcounter());
      if(diff >= 2) step((diff - 2) * 1364);
    }

    prev = next;
    step(2);
  }
}

uint2 Justifier::data() {
  if(counter >= 32) return 1;

  if(counter == 0) {
    trigger1 = interface->inputPoll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::Trigger);
    start1   = interface->inputPoll(port, Input::Device::Justifier, 0, (unsigned)Input::JustifierID::Start);
    if(chained) {
      trigger2 = interface->inputPoll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::Trigger);
      start2   = interface->inputPoll(port, Input::Device::Justifiers, 1, (unsigned)Input::JustifierID::Start);
    }
  }

  switch(counter++) {
  case  0: return 0;
  case  1: return 0;
  case  2: return 0;
  case  3: return 0;
  case  4: return 0;
  case  5: return 0;
  case  6: return 0;
  case  7: return 0;
  case  8: return 0;
  case  9: return 0;
  case 10: return 0;
  case 11: return 0;

  case 12: return 1;  //signature
  case 13: return 1;  // ||
  case 14: return 1;  // ||
  case 15: return 0;  // ||

  case 16: return 0;
  case 17: return 1;
  case 18: return 0;
  case 19: return 1;
  case 20: return 0;
  case 21: return 1;
  case 22: return 0;
  case 23: return 1;

  case 24: return trigger1;
  case 25: return trigger2;
  case 26: return start1;
  case 27: return start2;
  case 28: return active;

  case 29: return 0;
  case 30: return 0;
  case 31: return 0;
  }

  return 0;
}

void Justifier::latch(bool data) {
  if(latched == data) return;
  latched = data;
  counter = 0;
  if(latched == 0) active = !active;  //toggle between both controllers, even when unchained
}

void Justifier::serialize(serializer& s) {
  Processor::serialize(s);
  //Save block.
  uint8 block[Controller::SaveSize] = {0};
  block[0] = latched ? 1 : 0;
  block[1] = counter;
  block[2] = active ? 1 : 0;
  block[3] = trigger1 ? 1 : 0;
  block[4] = trigger2 ? 1 : 0;
  block[5] = start1 ? 1 : 0;
  block[6] = start2 ? 1 : 0;
  block[7] = (uint16)x1 >> 8;
  block[8] = ( int16)x1;
  block[9] = (uint16)x2 >> 8;
  block[10] = (uint16)x2;
  block[11] = (uint16)y1 >> 8;
  block[12] = (uint16)y1;
  block[13] = (uint16)y2 >> 8;
  block[14] = (uint16)y2;
  s.array(block, Controller::SaveSize);
  if(s.mode() == nall::serializer::Load) {
    latched = (block[0] != 0);
    counter = block[1];
    active = (block[2] != 0);
    trigger1 = (block[3] != 0);
    trigger2 = (block[4] != 0);
    start1 = (block[5] != 0);
    start2 = (block[6] != 0);
    x1 = (int16)(((uint16)block[7] << 8) | (uint16)block[8]);
    x2 = (int16)(((uint16)block[9] << 8) | (uint16)block[10]);
    y1 = (int16)(((uint16)block[11] << 8) | (uint16)block[12]);
    y2 = (int16)(((uint16)block[13] << 8) | (uint16)block[14]);
  }
}



Justifier::Justifier(bool port, bool chained) : Controller(port), chained(chained) {
  create(Controller::Enter, 21477272);
  latched = 0;
  counter = 0;
  active = 0;

  if(chained == false) {
    x1 = 256 / 2;
    y1 = 240 / 2;
    x2 = -1;
    y2 = -1;
  } else {
    x1 = 256 / 2 - 16;
    y1 = 240 / 2;
    x2 = 256 / 2 + 16;
    y2 = 240 / 2;
  }

  trigger1 = false;
  trigger2 = false;

  start1 = false;
  start2 = false;
}

#endif
