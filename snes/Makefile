snes_objects := libco
snes_objects += snes-interface snes-system snes-controller
snes_objects += snes-cartridge snes-cheat
snes_objects += snes-memory snes-cpucore snes-smpcore
snes_objects += snes-cpu snes-smp snes-dsp snes-ppu
snes_objects += snes-nss snes-icd2 snes-superfx snes-sa1 snes-necdsp snes-hitachidsp
snes_objects += snes-bsx snes-srtc snes-sdd1 snes-spc7110 snes-sufamiturbo
snes_objects += snes-obc1 snes-st0018
snes_objects += snes-msu1 snes-link
snes_objects += $(gameboy_objects)
objects += $(snes_objects)

ifeq ($(profile),accuracy)
  extraflags += -DPROFILE_ACCURACY
  snescpu := $(snes)/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/dsp
  snesppu := $(snes)/ppu
else ifeq ($(profile),compatibility)
  extraflags += -DPROFILE_COMPATIBILITY
  snescpu := $(snes)/cpu
  snessmp := $(snes)/smp
  snesdsp := $(snes)/alt/dsp
  snesppu := $(snes)/alt/ppu-compatibility
else ifeq ($(profile),performance)
  extraflags += -DPROFILE_PERFORMANCE
  snescpu := $(snes)/alt/cpu
  snessmp := $(snes)/alt/smp
  snesdsp := $(snes)/alt/dsp
  snesppu := $(snes)/alt/ppu-performance
endif

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	extraflags += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

obj/libretro.o: $(snes)/libretro/libretro.cpp $(snes)/libretro/*

obj/libco.o         : libco/libco.c libco/*

obj/snes-interface.o : $(snes)/interface/interface.cpp $(call rwildcard,$(snes)/interface)
obj/snes-system.o    : $(snes)/system/system.cpp $(call rwildcard,$(snes)/system/)
obj/snes-controller.o: $(snes)/controller/controller.cpp $(call rwildcard,$(snes)/controller/)
obj/snes-memory.o    : $(snes)/memory/memory.cpp $(call rwildcard,$(snes)/memory/)
obj/snes-cpucore.o   : $(snes)/cpu/core/core.cpp $(call rwildcard,$(snes)/cpu/core/)
obj/snes-smpcore.o   : $(snes)/smp/core/smp_core.cpp $(call rwildcard,$(snes)/smp/core/)
obj/snes-cpu.o       : $(snescpu)/cpu.cpp $(call rwildcard,$(snescpu)/)
obj/snes-smp.o       : $(snessmp)/smp.cpp $(call rwildcard,$(snessmp)/)
obj/snes-dsp.o       : $(snesdsp)/dsp.cpp $(call rwildcard,$(snesdsp)/)
obj/snes-ppu.o       : $(snesppu)/ppu.cpp $(call rwildcard,$(snesppu)/)
obj/snes-cartridge.o : $(snes)/cartridge/cartridge.cpp $(snes)/cartridge/*
obj/snes-cheat.o     : $(snes)/cheat/cheat.cpp $(snes)/cheat/*

obj/snes-nss.o     : $(snes)/chip/nss/nss.cpp $(call rwildcard,$(snes)/chip/nss/)
obj/snes-icd2.o    : $(snes)/chip/icd2/icd2.cpp $(call rwildcard,$(snes)/chip/icd2/)
obj/snes-superfx.o : $(snes)/chip/superfx/superfx.cpp $(call rwildcard,$(snes)/chip/superfx/)
obj/snes-sa1.o     : $(snes)/chip/sa1/sa1.cpp $(call rwildcard,$(snes)/chip/sa1/)
obj/snes-necdsp.o  : $(snes)/chip/necdsp/necdsp.cpp $(call rwildcard,$(snes)/chip/necdsp/)
obj/snes-hitachidsp.o : $(snes)/chip/hitachidsp/hitachidsp.cpp $(call rwildcard,$(snes)/chip/hitachidsp/)
obj/snes-bsx.o     : $(snes)/chip/bsx/bsx.cpp $(snes)/chip/bsx/*
obj/snes-srtc.o    : $(snes)/chip/srtc/srtc.cpp $(snes)/chip/srtc/*
obj/snes-sdd1.o    : $(snes)/chip/sdd1/sdd1.cpp $(snes)/chip/sdd1/*
obj/snes-spc7110.o : $(snes)/chip/spc7110/spc7110.cpp $(snes)/chip/spc7110/*
obj/snes-obc1.o    : $(snes)/chip/obc1/obc1.cpp $(snes)/chip/obc1/*
obj/snes-st0018.o  : $(snes)/chip/st0018/st0018.cpp $(snes)/chip/st0018/*
obj/snes-msu1.o    : $(snes)/chip/msu1/msu1.cpp $(snes)/chip/msu1/*
obj/snes-link.o    : $(snes)/chip/link/link.cpp $(snes)/chip/link/*
obj/snes-sufamiturbo.o: $(snes)/chip/sufamiturbo/sufamiturbo.cpp $(snes)/chip/sufamiturbo/*

###########
# library #
###########

snes_objects := $(patsubst %,obj/%.o,$(snes_objects))

static-library: obj/libretro.o $(snes_objects)
	ar rcs out/bsnes_cplusplus98_libretro.a obj/libretro.o $(snes_objects)

library: obj/libretro.o $(snes_objects)
ifeq ($(platform),x)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro.so -fPIC -shared -Wl,-soname,bsnes_cplusplus98_libretro.so.1 obj/libretro.o $(snes_objects) -ldl -Wl,-version-script=link.T -Wl,-no-undefined
else ifeq ($(platform),unix)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro.so -fPIC -shared -Wl,-soname,bsnes_cplusplus98_libretro.so.1 obj/libretro.o $(snes_objects) -ldl -Wl,-version-script=link.T -Wl,-no-undefined
else ifeq ($(platform),xbsd)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro.so -fPIC -shared -Wl,-soname,bsnes_cplusplus98_libretro.so.1 obj/libretro.o $(snes_objects) -lc -Wl,-version-script=link.T -Wl,-no-undefined
else ifeq ($(platform),haiku)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro.so -fPIC -shared -Wl,-soname,bsnes_cplusplus98_libretro.so.1 obj/libretro.o $(snes_objects) -lroot -Wl,-version-script=link.T -Wl,-no-undefined
else ifeq ($(platform),osx)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro.dylib -install_name $(prefix)/lib/bsnes_cplusplus98_libretro.dylib -dynamiclib obj/libretro.o $(snes_objects) $(link)
else ifeq ($(platform),ios-arm64)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro_ios.dylib -install_name $(prefix)/lib/bsnes_cplusplus98_libretro_ios.dylib -dynamiclib obj/libretro.o $(snes_objects) $(link)
else ifeq ($(platform),ios)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro_ios.dylib -install_name $(prefix)/lib/bsnes_cplusplus98_libretro_ios.dylib -dynamiclib obj/libretro.o $(snes_objects) $(link)
else ifeq ($(platform),qnx)
	ar rcs out/bsnes_cplusplus98_libretro.a $(snes_objects) obj/libretro.o
	$(CXX) -o out/bsnes_cplusplus98_libretro_qnx.so -fPIC -shared -Wl,-soname,bsnes_cplusplus98_libretro_qnx.so.1 obj/libretro.o $(snes_objects) -lm -Wl,-version-script=link.T -Wl,-no-undefined
else ifeq ($(platform),win)
	$(CXX) -o out/bsnes_cplusplus98_libretro.dll -static-libgcc -static-libstdc++ -shared -Wl,--out-implib,libretro.a obj/libretro.o $(snes_objects) $(link) -Wl,-version-script=link.T -Wl,-no-undefined
endif
