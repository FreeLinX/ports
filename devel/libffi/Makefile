# FreeLinX/ports - devel/libffi : Foreign Function Interface library (3.4.6).
#
# Portable, high level programming interface to various calling conventions.
# MIT license. Autoconf build.

NAME    := libffi
VERSION := 3.4.6
CATEGORY:= devel
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

LIBFFI_PREFIX = $(FREELINX_BUILD_DIR)/deps/libffi

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libffi-$(VERSION)
PROJECT_BIN = $(LIBFFI_PREFIX)/lib/libffi.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBFFI_PREFIX)" \
		--enable-static \
		--disable-shared \
		--disable-docs \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libffi.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(LIBFFI_PREFIX)/include/ffi.h" "$(STAGE_ROOT)/usr/include/ffi.h"
	@$(INSTALL) -m 644 "$(LIBFFI_PREFIX)/include/ffi_common.h" "$(STAGE_ROOT)/usr/include/ffi_common.h"
	@$(INSTALL) -m 644 "$(LIBFFI_PREFIX)/include/ffitarget.h" "$(STAGE_ROOT)/usr/include/ffitarget.h"
	@printf '[FreeLinX/ports] staged libffi\n'
