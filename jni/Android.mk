LOCAL_PATH := $(call my-dir)

SRCDIR := $(LOCAL_PATH)/..

INCFLAGS  := -I$(SRCDIR)
COREFLAGS := -fomit-frame-pointer -ffast-math -D__LIBRETRO__ $(INCFLAGS)
COREFLAGS += -Wno-unknown-attributes

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
  COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

SRCFILES := $(SRCDIR)/libco/libco.c \
            $(SRCDIR)/snes/interface/interface.cpp \
            $(SRCDIR)/snes/system/system.cpp \
            $(SRCDIR)/snes/controller/controller.cpp \
            $(SRCDIR)/snes/cartridge/cartridge.cpp \
            $(SRCDIR)/snes/cheat/cheat.cpp \
            $(SRCDIR)/snes/memory/memory.cpp \
            $(SRCDIR)/snes/chip/icd2/icd2.cpp \
            $(SRCDIR)/snes/chip/bsx/bsx.cpp \
            $(SRCDIR)/snes/chip/sufamiturbo/sufamiturbo.cpp \
            $(SRCDIR)/snes/chip/bsx/satellaview/satellaview.cpp \
            $(SRCDIR)/snes/chip/nss/nss.cpp \
            $(SRCDIR)/snes/chip/sa1/sa1.cpp \
            $(SRCDIR)/snes/chip/superfx/superfx.cpp \
            $(SRCDIR)/snes/chip/hitachidsp/hitachidsp.cpp \
            $(SRCDIR)/snes/chip/link/link.cpp \
            $(SRCDIR)/snes/chip/necdsp/necdsp.cpp \
            $(SRCDIR)/snes/chip/spc7110/spc7110.cpp \
            $(SRCDIR)/snes/chip/st0018/st0018.cpp \
            $(SRCDIR)/snes/chip/sdd1/sdd1.cpp \
            $(SRCDIR)/snes/chip/obc1/obc1.cpp \
            $(SRCDIR)/snes/chip/msu1/msu1.cpp \
            $(SRCDIR)/snes/chip/srtc/srtc.cpp \
            $(SRCDIR)/snes/cpu/core/core.cpp \
            $(SRCDIR)/snes/cpu/dma/dma.cpp \
            $(SRCDIR)/snes/cpu/memory/memory.cpp \
            $(SRCDIR)/snes/cpu/mmio/mmio.cpp \
            $(SRCDIR)/snes/cpu/timing/timing.cpp \
            $(SRCDIR)/gameboy/interface/interface_gb.cpp \
            $(SRCDIR)/gameboy/system/system_gb.cpp \
            $(SRCDIR)/gameboy/scheduler/scheduler_gb.cpp \
            $(SRCDIR)/gameboy/memory/memory_gb.cpp \
            $(SRCDIR)/gameboy/cartridge/cartridge_gb.cpp \
            $(SRCDIR)/gameboy/cpu/cpu_gb.cpp \
            $(SRCDIR)/gameboy/apu/apu_gb.cpp \
            $(SRCDIR)/gameboy/cheat/cheat_gb.cpp \
            $(SRCDIR)/gameboy/lcd/lcd_gb.cpp \
            $(SRCDIR)/gameboy/video/video_gb.cpp \
            $(SRCDIR)/snes/libretro/libretro.cpp

# Other platforms force the performance profile, so replicate here
COREFLAGS += -DPROFILE_PERFORMANCE
SRCFILES += $(SRCDIR)/snes/alt/cpu/cpu.cpp \
            $(SRCDIR)/snes/alt/smp/smp.cpp \
            $(SRCDIR)/snes/alt/dsp/dsp.cpp \
            $(SRCDIR)/snes/alt/ppu-performance/ppu.cpp

include $(CLEAR_VARS)
LOCAL_MODULE       := retro
LOCAL_SRC_FILES    := $(SRCFILES)
LOCAL_CPPFLAGS     := $(COREFLAGS)
LOCAL_CFLAGS       := $(COREFLAGS)
LOCAL_LDFLAGS      := -Wl,-version-script=$(SRCDIR)/link.T
LOCAL_CPP_FEATURES := exceptions rtti
include $(BUILD_SHARED_LIBRARY)
