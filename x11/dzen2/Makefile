# FreeLinX/ports - x11/dzen2 : general purpose messaging and notification program (0.9.5).
#
# XFT-based lightweight top bar for the Openbox desktop (clock/date/hostname).
# MIT license.  Plain POSIX make build.  Requires libXft/libX11 plus the
# freetype/fontconfig stack from the X11 dependency tree.

NAME    := dzen2
VERSION := 0.9.5
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= X11 libs (libXft, libX11, build/deps/x11)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dzen2
INSTALL_RELPATH:= usr/bin/dzen2

include ../../mk/project-port.mk

X11_PREFIX=$(FREELINX_PORTS_ROOT)/../desktop-test/ports/build/deps/x11

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dzen-master
PROJECT_BIN = $(SRC_TREE)/dzen2

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="-O2 -I$(X11_PREFIX)/include -I$(X11_PREFIX)/include/freetype2 -DVERSION='\"0.9.5\"' -DDZEN_XFT" \
		LDFLAGS="-L$(X11_PREFIX)/lib -lXft -lX11 -lXrender -lfontconfig -lfreetype -lexpat -lz -lXau -lXdmcp -lxcb -lm -static"