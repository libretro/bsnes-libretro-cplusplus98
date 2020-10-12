LOCAL_PATH := $(call my-dir)

CORE_DIR := $(LOCAL_PATH)/..

FRONTEND_SUPPORTS_RGB565 := 1
PROFILE                  := performance
FLAGS                    := -DPROFILE_PERFORMANCE

ifeq ($(TARGET_ARCH),x86)
  IS_X86 := 1
endif

include $(CORE_DIR)/Makefile.common

COREFLAGS := -fomit-frame-pointer $(INCFLAGS) -D__LIBRETRO__ $(FLAGS) -Wno-unknown-attributes

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
  COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

include $(CLEAR_VARS)
LOCAL_MODULE       := retro
LOCAL_SRC_FILES    := $(SOURCES_CXX) $(SOURCES_C)
LOCAL_CFLAGS       := $(COREFLAGS)
LOCAL_CXXFLAGS     := $(COREFLAGS)
LOCAL_LDFLAGS      := -Wl,-version-script=$(CORE_DIR)/link.T
LOCAL_CPP_FEATURES := exceptions rtti
include $(BUILD_SHARED_LIBRARY)
