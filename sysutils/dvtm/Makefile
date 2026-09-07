# FreeLinX/ports - sysutils/dvtm : dvtm 0.15 (dynamic virtual terminal manager).
#
# Dynamic virtual terminal manager for the console. Plain Makefile build.
# MIT license. Requires ncurses (base/ncurses).

NAME    := dvtm
VERSION := 0.15
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:= ncurses
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), ncurses (base/ncurses)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dvtm
INSTALL_RELPATH:= usr/bin/dvtm

NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/ncurses

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dvtm-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/dvtm
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -I$(NCURSES_PREFIX)/include -I$(NCURSES_PREFIX)/include/ncursesw" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib -lncursesw" \
		CPPFLAGS="-D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED" \
		dvtm
