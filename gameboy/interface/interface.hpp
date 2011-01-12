class Interface {
public:
  virtual void joyp_write(bool p15, bool p14) {}

  virtual void video_refresh(const uint8_t *data) {}
  virtual void audio_sample(signed left, signed right) {}
  virtual void input_poll() {}
  virtual bool input_poll(unsigned id) { return true; }

  virtual void message(const string &text) { print(text, "\n"); }
};
