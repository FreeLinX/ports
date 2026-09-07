# FreeLinX/ports - graphics/pixman : pixel manipulation library (0.43.4).
#
# Low-level pixel manipulation library, required by cairo.
# MIT license. Autoconf build.

NAME    := pixman
VERSION := 0.43.4
CATEGORY:= graphics
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

PIXMAN_PREFIX = $(FREELINX_BUILD_DIR)/deps/pixman

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/pixman-$(VERSION)
PROJECT_BIN = $(PIXMAN_PREFIX)/lib/libpixman-1.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(PIXMAN_PREFIX)" \
		--disable-shared \
		--enable-static \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libpixman-1.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/pixman-1"
	@cp "$(PIXMAN_PREFIX)/include/pixman-1/pixman.h" "$(STAGE_ROOT)/usr/include/pixman-1/pixman.h"
	@cp "$(PIXMAN_PREFIX)/include/pixman-1/pixman-version.h" "$(STAGE_ROOT)/usr/include/pixman-1/pixman-version.h"
	@printf '[FreeLinX/ports] staged pixman\n'
