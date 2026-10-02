# FreeLinX/ports - audio/tinyalsa : minimal non-GNU ALSA audio suite.
#
# Metadata:
#   NAME       tinyalsa
#   CATEGORY   audio
#   LICENSE    BSD-3-Clause
#   INSTALL    tinymix, tinyplay, tinycap, tinypcminfo -> FreeLinX/src/rootfs/usr/bin/
#
# It also publishes a build-dependency prefix at build/deps/tinyalsa, holding
# nothing but <tinyalsa/*.h> and libtinyalsa.a, for audio/mpg123 to compile and
# link against.
#
# The alternative - telling mpg123 -I/usr/include -L/usr/lib - is how the host's
# glibc gets into a musl build.  -L/usr/lib finds /usr/lib/libc.a before the
# musl sysroot's, and the link fails with
#
#   ld.lld: error: undefined symbol: _Unwind_Resume
#   >>> referenced by iofputs.o:(_IO_fputs.cold) in archive /usr/lib/libc.a
#   ld.lld: error: undefined symbol: __gcc_personality_v0
#
# because glibc's static libc needs libgcc_eh.a, which this toolchain has no
# equivalent of on that path.  Worse than a build failure: had it linked, the
# result would be a mixed glibc/musl binary, which is the one thing this tree
# must never ship.  A prefix with only tinyalsa in it cannot do either.

NAME    := tinyalsa
VERSION := 2.0.0
CATEGORY:= audio
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:=

INSTALL_BIN     := tinymix
INSTALL_RELPATH := usr/bin/tinymix

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))../..)
DRIVERS_DIR := $(abspath $(FREELINX_PORTS_ROOT)/../drivers/audio/tinyalsa)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

# `=`, not `:=`: FREELINX_BUILD_DIR is only bound in common.mk, below.
TINYALSA_PREFIX = $(FREELINX_BUILD_DIR)/deps/tinyalsa

.PHONY: all do-build install clean

all: do-build

do-build:
	@printf '[FreeLinX/ports] building audio/tinyalsa via drivers/audio/tinyalsa\n'
	$(MAKE) -C "$(DRIVERS_DIR)" all

install: do-build
	@printf '[FreeLinX/ports] installing tinyalsa utilities to staging\n'
	$(MKDIR) -p "$(STAGE_ROOT)/usr/bin" "$(STAGE_ROOT)/usr/lib" "$(STAGE_ROOT)/usr/include/tinyalsa"
	$(MAKE) -C "$(DRIVERS_DIR)" install DESTDIR="$(STAGE_ROOT)"
	@printf '[FreeLinX/ports] publishing tinyalsa into %s\n' "$(TINYALSA_PREFIX)"
	$(MKDIR) -p "$(TINYALSA_PREFIX)/include/tinyalsa" "$(TINYALSA_PREFIX)/lib"
	cp "$(STAGE_ROOT)"/usr/include/tinyalsa/*.h "$(TINYALSA_PREFIX)/include/tinyalsa/"
	cp "$(STAGE_ROOT)"/usr/lib/libtinyalsa.a "$(TINYALSA_PREFIX)/lib/"
	@test -f "$(TINYALSA_PREFIX)/lib/libtinyalsa.a" || { \
		printf '[FreeLinX/ports][error] tinyalsa: libtinyalsa.a did not reach %s\n' "$(TINYALSA_PREFIX)" >&2; \
		exit 1; \
	}

install-rootfs: install
	@printf '[FreeLinX/ports] installing tinyalsa into %s\n' "$(ROOTFS_DIR)"
	$(MAKE) -C "$(DRIVERS_DIR)" install DESTDIR="$(ROOTFS_DIR)"

clean:
	$(MAKE) -C "$(DRIVERS_DIR)" clean
