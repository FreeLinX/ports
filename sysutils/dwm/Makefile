# FreeLinX/ports - sysutils/dwm : suckless dynamic window manager.
#
# Dynamic window manager for X.  MIT license.  Plain POSIX make build.
# Requires X11 libraries (libX11, libXft) from the X11 dependency tree.
# Treat as an X11 port; lives in sysutils/ for organizational reasons.

NAME    := dwm
VERSION := 6.5
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= X11 libs (build/deps/X11, future)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dwm
INSTALL_RELPATH:= usr/bin/dwm

include ../../mk/project-port.mk

X11_PREFIX=$(FREELINX_BUILD_DIR)/deps/X11

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dwm-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/dwm

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -I$(X11_PREFIX)/include" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt -L$(X11_PREFIX)/lib -lX11 -lXft -lm"
