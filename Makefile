include nall/Makefile
snes := snes
profile := performance

# compiler

CELL_SDK := /usr/local/cell
ifeq ($(platform),win)
   HOST_DIR := host-win32
else
   HOST_DIR := host-linux
endif
# SDK compiler
compiler := $(CELL_SDK)/$(HOST_DIR)/ppu/bin/ppu-lv2-gcc

c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler))
as      := $(subst gcc,as,$(compiler))
#flags   := -O2 -I. -I$(snes) -DLIBSNES_DEBUG -DLIBCO_PPC_FP -DLIBCO_PPC_ALTIVEC
flags   := -O3 -I. -I$(snes) -fstrict-aliasing -mcpu=cell
cxxflags := -fno-rtti -fno-exceptions
#flags   := -O2 -I. -I$(snes)
#flags   := -g -I. -I$(snes) -fpic
link    :=
objects :=

# profile-guided instrumentation
# flags += -fprofile-generate
# link += -lgcov

# profile-guided optimization
# flags += -fprofile-use

# implicit rules
compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(c) $(flags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(cpp) $(flags) $(cxxflags) $1 -c $< -o $@, \
		  $(if $(filter %.s,$<), \
		    $(as) $< -o $@ \
		  ) \
      ) \
    ) \
  )

all: library;

install: library-install;

uninstall: library-uninstall;

%.o: $<; $(call compile)
include $(snes)/Makefile

clean: 
	-@$(call delete,obj/*.o)
	-@$(call delete,obj/*.a)
	-@$(call delete,out/*.a)
	-@$(call delete,obj/*.so)
	-@$(call delete,obj/*.dylib)
	-@$(call delete,obj/*.dll)
	-@$(call delete,*.res)
	-@$(call delete,*.pgd)
	-@$(call delete,*.pgc)
	-@$(call delete,*.ilk)
	-@$(call delete,*.pdb)
	-@$(call delete,*.manifest)

archive-all:
	tar -cjf bsnes.tar.bz2 libco nall obj out snes Makefile cc.bat clean.bat sync.sh

help:;
