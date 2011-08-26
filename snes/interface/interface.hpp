#ifndef SNES_INTERFACE_HPP
#define SNES_INTERFACE_HPP

#include <snes/libsnes/libsnes.hpp>

struct Interface {
  Interface();

  void video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan);
  void audio_sample(int16_t l_sample, int16_t r_sample);
  int16_t input_poll(bool port, Input::Device::e device, unsigned index, unsigned id);

  void message(const string &text);
  string path(Cartridge::Slot::e slot, const string &hint);

  snes_video_refresh_t pvideo_refresh;
  snes_audio_sample_t paudio_sample;
  snes_input_poll_t pinput_poll;
  snes_input_state_t pinput_state;
  string basename;
};

#endif
