# FreeLinX/ports - x11/xclock : X clock display.
#
# Simple X clock display.  MIT license.  Autoconf-based build.
# Requires X11 libraries (libX11) from the X11 dependency tree.

NAME    := xclock
VERSION := 1.1.1
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= X11 libs (build/deps/X11, future)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= xclock
INSTALL_RELPATH:= usr/bin/xclock

include ../../mk/project-port.mk

X11_PREFIX=$(FREELINX_BUILD_DIR)/deps/X11

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/xclock-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/xclock

PROJECT_CFG_CMDS = \
	./configure --host=$(FREELINX_TRIPLE) --prefix=/usr \
		--disable-shared --enable-static \
		X11_CFLAGS="-I$(X11_PREFIX)/include" \
		X11_LIBS="-L$(X11_PREFIX)/lib -lX11"

BUILD_CMDS = \
	$(MAKE) -C "$(SRC_TREE)"
