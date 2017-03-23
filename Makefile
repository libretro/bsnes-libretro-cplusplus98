include nall/Makefile
snes := snes
gameboy := gameboy
profile := performance
link :=

ifeq ($(platform),)
# see nall/Makefile and nall/detect.hpp
platform = x
ifeq ($(shell uname -a),)
   platform = win
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   platform = osx
   arch = intel
ifeq ($(shell uname -p),powerpc)
   arch = ppc
endif
else ifneq ($(findstring win,$(shell uname -a)),)
   platform = win
endif
endif

ifeq ($(platform),unix)
   platform = x
   fpic = -fPIC
endif

ifeq ($(platform),x)
   fpic = -fPIC
else ifeq ($(platform),win)
   CC = gcc
   CXX = g++
else ifeq ($(platform),osx)
   fpic = -fPIC
   OSXVER = `sw_vers -productVersion | cut -d. -f 2`
   OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
ifeq ($(OSX_LT_MAVERICKS),"YES")
   fpic += -mmacosx-version-min=10.5
endif
else ifneq (,$(findstring ios,$(platform)))
   fpic = -fPIC
   CC = cc -arch armv7 -isysroot $(IOSSDK)
   CXX =  c++ -arch armv7 -isysroot $(IOSSDK)
ifeq ($(platform),ios9)
   CC += -miphoneos-version-min=8.0
   CXX += -miphoneos-version-min=8.0
else
   CC += -miphoneos-version-min=5.0
   CXX += -miphoneos-version-min=5.0
endif
else ifeq ($(platform),qnx)
   fpic = -fPIC
   CC = qcc -Vgcc_ntoarmv7le
   CXX = QCC -Vgcc_ntoarmv7le_cpp
endif

ifeq ($(DEBUG),1)
   extraflags = -O0 -g -I. -I$(snes) $(fpic)
else
   extraflags = -O3 -fomit-frame-pointer -I. -I$(snes) $(fpic)
endif

CXXFLAGS += -std=gnu++98

ifeq ($(PROFILING),gen)
   extraflags += -fprofile-generate --coverage
   link += -fprofile-generate --coverage
endif

ifeq ($(PROFILING),use)
   extraflags += -fprofile-use
   link += -fprofile-use
endif

ifeq ($(platform),osx)
   ifndef ($(NOUNIVERSAL))
      extraflags += $(ARCHFLAGS)
      link += $(ARCHFLAGS)
   endif
endif

ifneq (,$(findstring ios,$(platform)))
   extraflags += -DHAVE_POSIX_MEMALIGN=1 -marm
endif

# implicit rules
compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(CC) $(CFLAGS) $(extraflags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(CXX) $(CXXFLAGS) $(extraflags) $1 -c $< -o $@, \
		  $(if $(filter %.s,$<), \
		    $(AS) $(ASFLAGS) -o $@ $<, \
      	) \
		 ) \
    ) \
  )

all: library;

include gameboy/Makefile


set-static:
ifneq ($(platform),win)
	$(eval fpic := )
endif

static: set-static static-library;

install: library-install;

uninstall: library-uninstall;

%.o: $<; $(call compile)
include $(snes)/Makefile

clean: 
	-@$(call delete,obj/*.o)
	-@$(call delete,obj/*.a)
	-@$(call delete,obj/*.so)
	-@$(call delete,obj/*.dylib)
	-@$(call delete,obj/*.dll)
	-@$(call delete,out/*.a)
	-@$(call delete,out/*.so)
	-@$(call delete,*.res)
	-@$(call delete,*.pgd)
	-@$(call delete,*.pgc)
	-@$(call delete,*.ilk)
	-@$(call delete,*.pdb)
	-@$(call delete,*.manifest)

archive-all:
	tar -cjf libsnes.tar.bz2 libco nall obj out snes Makefile cc.bat clean.bat sync.sh

help:;
