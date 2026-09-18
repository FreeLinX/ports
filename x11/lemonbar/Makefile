# FreeLinX/ports - x11/lemonbar : Lemonbar 1.4-xft status bar (static).
#
# Bottom status bar for the Openbox desktop (CPU/RAM/disk/temp/clock).
# Fork with Xft (fontconfig) support, in sync with Oct 2024 lemonbar.
# MIT license.  Plain POSIX make build.  Requires libxcb + libX11 + libXft.

NAME    := lemonbar
VERSION := 1.4
CATEGORY:= x11
LICENSE := MIT
PKG_DEPENDS := fonts
DEPENDENCIES:=
BUILD_DEPENDENCIES:= X11 libs (libxcb, libX11, libXft, build/deps/x11)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= lemonbar
INSTALL_RELPATH:= usr/bin/lemonbar

include ../../mk/project-port.mk

X11_PREFIX=$(FREELINX_PORTS_ROOT)/../desktop-test/ports/build/deps/x11

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/lemonbar-xft-master
PROJECT_BIN = $(SRC_TREE)/lemonbar

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="-O2 -std=c99 -Wall -DVERSION='\"1.4-xft\"' -D_GNU_SOURCE -I$(X11_PREFIX)/include -I$(X11_PREFIX)/include/freetype2" \
		LDFLAGS="-L$(X11_PREFIX)/lib -lxcb -lxcb-xinerama -lxcb-randr -lX11 -lXft -lX11-xcb -lXrender -lfontconfig -lfreetype -lexpat -lz -lXau -lXdmcp -lm -static"