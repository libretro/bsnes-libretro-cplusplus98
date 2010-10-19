class Interface;

class System : property<System> {
public:
  struct Region{ enum e{ NTSC = 0, PAL = 1, Autodetect = 2 } i; };
  struct ExpansionPortDevice{ enum e{ None = 0, BSX = 1 } i; };

  void run();
  void runtosave();

  void init(Interface*);
  void term();
  void power();
  void reset();
  void unload();

  void frame();
  void scanline();

  //return *active* system information (settings are cached upon power-on)
  Region region;
  ExpansionPortDevice expansion;
  readonly<uint64_t> cpu_frequency;
  readonly<uint64_t> apu_frequency;
  readonly<uint64_t> serialize_size;

  serializer serialize();
  bool unserialize(serializer&);

  System();

private:
  Interface *interface;
  void runthreadtosave();

  void serialize(serializer&);
  void serialize_all(serializer&);
  void serialize_init();

  friend class Cartridge;
  friend class Video;
  friend class Audio;
  friend class Input;
};

#include <video/video.hpp>
#include <audio/audio.hpp>
#include <input/input.hpp>

#include <config/config.hpp>
#include <debugger/debugger.hpp>
#include <interface/interface.hpp>
#include <scheduler/scheduler.hpp>

extern System system;
