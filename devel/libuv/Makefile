# FreeLinX/ports - devel/libuv : multi-platform async I/O library (1.49.1).
#
# Cross-platform asynchronous I/O. Used by Node.js and other event-driven
# applications. MIT license. Autoconf build.

NAME    := libuv
VERSION := 1.49.1
CATEGORY:= devel
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

LIBUV_PREFIX = $(FREELINX_BUILD_DIR)/deps/libuv

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libuv-v$(VERSION)
PROJECT_BIN = $(LIBUV_PREFIX)/lib/libuv.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBUV_PREFIX)" \
		--enable-static \
		--disable-shared \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libuv.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(LIBUV_PREFIX)/include/uv.h" "$(STAGE_ROOT)/usr/include/uv.h"
	@$(INSTALL) -m 644 "$(LIBUV_PREFIX)/include/uv/*.h" "$(STAGE_ROOT)/usr/include/uv/"
	@printf '[FreeLinX/ports] staged libuv\n'
