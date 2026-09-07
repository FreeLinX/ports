# FreeLinX/ports - graphics/libwebp : WebP image library (1.4.0).
#
# WebP encoding/decoding library. BSD-3-Clause license. Autoconf build.

NAME    := libwebp
VERSION := 1.4.0
CATEGORY:= graphics
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

WEBP_PREFIX = $(FREELINX_BUILD_DIR)/deps/libwebp

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libwebp-$(VERSION)
PROJECT_BIN = $(WEBP_PREFIX)/lib/libwebp.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(WEBP_PREFIX)" \
		--enable-static \
		--disable-shared \
		--disable-gl \
		--disable-sdl \
		--disable-png \
		--disable-jpeg \
		--disable-tiff \
		--disable-gif \
		--disable-threading \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libwebp.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/webp"
	@cp "$(SRC_TREE)/src/webp/decode.h" "$(STAGE_ROOT)/usr/include/webp/decode.h"
	@cp "$(SRC_TREE)/src/webp/encode.h" "$(STAGE_ROOT)/usr/include/webp/encode.h"
	@cp "$(SRC_TREE)/src/webp/types.h" "$(STAGE_ROOT)/usr/include/webp/types.h"
	@cp "$(SRC_TREE)/src/webp/mux.h" "$(STAGE_ROOT)/usr/include/webp/mux.h"
	@printf '[FreeLinX/ports] staged libwebp\n'
