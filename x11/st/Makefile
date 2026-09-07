# FreeLinX/ports - x11/st : suckless terminal emulator.
#
# Simple terminal emulator for X.  MIT license.  Plain POSIX make build.
# Requires X11 libraries (libX11, libXft) from the X11 dependency tree.

NAME    := st
VERSION := 0.9.2
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= X11 libs (build/deps/X11, future)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= st
INSTALL_RELPATH:= usr/bin/st

include ../../mk/project-port.mk

X11_PREFIX=$(FREELINX_BUILD_DIR)/deps/X11

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/st-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/st

ST_CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -I$(X11_PREFIX)/include"
ST_LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt -lX11 -lfreetype -lm"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="$(ST_CFLAGS)" \
		LDFLAGS="$(ST_LDFLAGS)"
