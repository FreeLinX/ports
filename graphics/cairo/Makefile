# FreeLinX/ports - graphics/cairo : 2D graphics library (1.18.0).
#
# Cairo 2D rendering library. LGPL-2.1-only/MPL-1.1 dual license.
# Autoconf build. Depends on pixman, freetype.

NAME    := cairo
VERSION := 1.18.0
CATEGORY:= graphics
LICENSE := LGPL-2.1-only
DEPENDENCIES:= pixman freetype2
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

CAIRO_PREFIX = $(FREELINX_BUILD_DIR)/deps/cairo
PIXMAN_PREFIX = $(FREELINX_BUILD_DIR)/deps/pixman

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/cairo-$(VERSION)
PROJECT_BIN = $(CAIRO_PREFIX)/lib/libcairo.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(CAIRO_PREFIX)" \
		--enable-static \
		--disable-shared \
		--enable-pdf=no \
		--enable-ps=no \
		--enable-svg=no \
		--disable-xlib \
		--disable-xcb \
		--disable-qt \
		--disable-quartz \
		--disable-win32 \
		--enable-freetype \
		--enable-png=no \
		--with-pixman-prefix="$(PIXMAN_PREFIX)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libcairo.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/cairo"
	@cp "$(CAIRO_PREFIX)/include/cairo/cairo.h" "$(STAGE_ROOT)/usr/include/cairo/cairo.h"
	@cp "$(CAIRO_PREFIX)/include/cairo/cairo-script.h" "$(STAGE_ROOT)/usr/include/cairo/cairo-script.h"
	@cp "$(CAIRO_PREFIX)/include/cairo/cairo-version.h" "$(STAGE_ROOT)/usr/include/cairo/cairo-version.h"
	@printf '[FreeLinX/ports] staged cairo\n'
