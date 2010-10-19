class SRTC : public MMIO {
public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 mmio_read(uint64_t addr);
  void mmio_write(uint64_t addr, uint8 data);

  void serialize(serializer&);
  SRTC();

private:
  static const uint64_t months[12];
  enum RtcMode { RtcReady, RtcCommand, RtcRead, RtcWrite };
  uint64_t rtc_mode;
  signed rtc_index;

  void update_time();
  uint64_t weekday(uint64_t year, uint64_t month, uint64_t day);
};

extern SRTC srtc;
