# FreeLinX/ports - sysutils/ncdu : ncdu 1.20 (ncurses disk usage analyzer).
#
# Interactive disk usage analyzer with ncurses interface. Plain Makefile
# build (custom configure). MIT license.

NAME    := ncdu
VERSION := 1.20
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= ncdu
INSTALL_RELPATH:= usr/bin/ncdu

NCURSES_PREFIX=$(FREELINX_BUILD_DIR)/deps/ncurses

FLX_PROJECT_CPPFLAGS = -I$(NCURSES_PREFIX)/include -I$(NCURSES_PREFIX)/include/ncursesw

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/ncdu-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/ncdu
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure --prefix=/usr

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -I$(NCURSES_PREFIX)/include -I$(NCURSES_PREFIX)/include/ncursesw" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib -lncursesw -ltinfo"
