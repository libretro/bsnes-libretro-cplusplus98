class Interface {
public:
  virtual void video_refresh(const uint16_t *data, uint64_t width, uint64_t height) {}
  virtual void audio_sample(uint16_t l_sample, uint16_t r_sample) {}
  virtual void input_poll() {}
  virtual int16_t input_poll(bool port, Input::Device device, uint64_t index, uint64_t id) { return 0; }
};
