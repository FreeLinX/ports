# FreeLinX/ports - sysutils/fdupes : fdupes 2.3.2 (duplicate file finder).
#
# Finds duplicate files in given sets of directories. Uses a configure
# script (not autoconf). MIT license.

NAME    := fdupes
VERSION := 2.3.2
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= fdupes
INSTALL_RELPATH:= usr/bin/fdupes

SQLITE_PREFIX=$(FREELINX_BUILD_DIR)/deps/sqlite

FLX_PROJECT_CPPFLAGS = -I$(SQLITE_PREFIX)/include

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/fdupes-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/fdupes
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--without-ncurses \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(SQLITE_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(SQLITE_PREFIX)/lib"

BUILD_CMDS = \
	$(MAKE) -j"$(FREELINX_JOBS)"
