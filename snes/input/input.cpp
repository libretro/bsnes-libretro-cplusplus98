#ifdef SYSTEM_CPP

Input input;

void Input::connect(bool port, Input::Device::e id) {
  Controller *&controller = ((port ? 1 : 0) == Controller::Port1 ? port1 : port2);
  if(controller) {
    delete controller;
    controller = 0;
  }

  switch(id) { default:
  case Device::None: controller = new Controller(port); break;
  case Device::Joypad: controller = new Gamepad(port); break;
  case Device::Multitap: controller = new Multitap(port); break;
  case Device::Mouse: controller = new Mouse(port); break;
  case Device::SuperScope: controller = new SuperScope(port); break;
  case Device::Justifier: controller = new Justifier(port, false); break;
  case Device::Justifiers: controller = new Justifier(port, true); break;
  case Device::Serial: controller = new Serial(port); break;
  }

  switch(port ? 1 : 0) {
  case Controller::Port1: config.controller_port1.i = id; break;
  case Controller::Port2: config.controller_port2.i = id; break;
  }
}

void Input::serialize(serializer &s) {
  unsigned p1 = config.controller_port1.i;
  unsigned p2 = config.controller_port2.i;
  s.integer(p1);
  s.integer(p2);
  if(s.mode() == nall::serializer::Load) {
    connect(Controller::Port1, (Device::e)p1);
    connect(Controller::Port2, (Device::e)p2);
  }
  port1->serialize(s);
  port2->serialize(s);
}

Input::Input() : port1(0), port2(0) {
  connect(Controller::Port1, Input::Device::Joypad);
  connect(Controller::Port2, Input::Device::Joypad);
}

Input::~Input() {
  if(port1) delete port1;
  if(port2) delete port2;
}

#endif
