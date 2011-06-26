class Interface {
public:
  virtual void video_refresh(const uint16_t *data, bool hires, bool interlace, bool overscan) = 0;
  virtual void audio_sample(uint16_t l_sample, uint16_t r_sample) = 0;
  virtual void input_poll() = 0;
  virtual int16_t input_poll(bool port, Input::Device::e device, unsigned index, unsigned id) = 0;

  virtual void message(const string &text) { print(text, "\n"); }
  virtual string path(Cartridge::Slot::e slot, const string &hint) = 0;
};
