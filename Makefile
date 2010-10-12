include nall/Makefile
snes := snes
profile := performance

# compiler

# SDK compiler
compiler := wine /home/maister/bin/PS3_SDK_v1.92-FuxSony/host-win32/ppu/bin/ppu-lv2-gcc.exe

c       := $(compiler) -std=gnu99
cpp     := $(subst cc,++,$(compiler))
flags   := -O3 -fomit-frame-pointer -I. -I$(snes) -DLIBSNES_DEBUG
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
        $(cpp) $(flags) $1 -c $< -o $@ \
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
