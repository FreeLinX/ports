# FreeLinX/ports - sysutils/htop : htop 3.3.0 (interactive process viewer).
#
# Enhanced top-like process viewer. Autoconf build, GPL-2.0 license.
# Built without unicode and sensors support. May link against ncurses if
# available in build/deps/ncurses.

NAME    := htop
VERSION := 3.3.0
CATEGORY:= sysutils
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= htop
INSTALL_RELPATH:= usr/bin/htop

NCURSES_PREFIX=$(FREELINX_BUILD_DIR)/deps/ncurses

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/htop-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/htop
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./autogen.sh && \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		--disable-unicode \
		--without-sensors \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(NCURSES_PREFIX)/include -I$(NCURSES_PREFIX)/include/ncursesw" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib" \
		LIBS="-lncursesw -ltinfo"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
