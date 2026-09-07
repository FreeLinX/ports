# FreeLinX/ports - security/libgpg-error : libgpg-error 1.51 (GPG error handling).
#
# LGPL-2.1-licensed library defining common error values for all GnuPG
# components.  Prerequisite for libgcrypt.  Builds a static archive
# into a local prefix; the custom install: rule copies the archive and
# headers into the staging tree.

NAME    := libgpg-error
VERSION := 1.51
CATEGORY:= security
LICENSE := LGPL-2.1
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

GPG_ERROR_PREFIX = $(FREELINX_BUILD_DIR)/deps/libgpg-error

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libgpg-error-$(VERSION)
PROJECT_BIN = $(GPG_ERROR_PREFIX)/lib/libgpg-error.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--build="$$(./build-aux/config.guess)" \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(GPG_ERROR_PREFIX)" \
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
	@$(INSTALL) -m 644 "$(GPG_ERROR_PREFIX)/lib/libgpg-error.a" "$(STAGE_ROOT)/usr/lib/libgpg-error.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(GPG_ERROR_PREFIX)/include/gpg-error.h" "$(STAGE_ROOT)/usr/include/gpg-error.h"
	@printf '[FreeLinX/ports] staged libgpg-error\n'
