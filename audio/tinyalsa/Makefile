# FreeLinX/ports - audio/tinyalsa : minimal non-GNU ALSA audio suite.
#
# Metadata:
#   NAME       tinyalsa
#   CATEGORY   audio
#   LICENSE    BSD-3-Clause
#   INSTALL    tinymix, tinyplay, tinycap, tinypcminfo -> FreeLinX/src/rootfs/usr/bin/

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

.PHONY: all do-build install clean

all: do-build

do-build:
	@printf '[FreeLinX/ports] building audio/tinyalsa via drivers/audio/tinyalsa\n'
	$(MAKE) -C "$(DRIVERS_DIR)" all

install: do-build
	@printf '[FreeLinX/ports] installing tinyalsa utilities to staging\n'
	$(MKDIR) -p "$(STAGE_ROOT)/usr/bin" "$(STAGE_ROOT)/usr/lib" "$(STAGE_ROOT)/usr/include/tinyalsa"
	$(MAKE) -C "$(DRIVERS_DIR)" install DESTDIR="$(STAGE_ROOT)"

install-rootfs: install
	@printf '[FreeLinX/ports] installing tinyalsa into %s\n' "$(ROOTFS_DIR)"
	$(MAKE) -C "$(DRIVERS_DIR)" install DESTDIR="$(ROOTFS_DIR)"

clean:
	$(MAKE) -C "$(DRIVERS_DIR)" clean
