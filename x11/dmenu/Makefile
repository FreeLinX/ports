# FreeLinX/ports - x11/dmenu : suckless dynamic menu.
#
# Dynamic menu for X, similar to dzen2.  MIT license.  Plain POSIX make build.
# Requires X11 libraries (libX11, libXft) from the X11 dependency tree.

NAME    := dmenu
VERSION := 5.3
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= X11 libs (build/deps/X11, future)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dmenu
INSTALL_RELPATH:= usr/bin/dmenu

include ../../mk/project-port.mk

X11_PREFIX=$(FREELINX_BUILD_DIR)/deps/X11

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dmenu-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/dmenu

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -I$(X11_PREFIX)/include" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt -L$(X11_PREFIX)/lib -lX11 -lXft"
