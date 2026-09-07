# FreeLinX/ports - security/libsodium : libsodium 1.0.20 (cryptography library).
#
# ISC-licensed modern, easy-to-use software encryption library.  The build
# produces a static archive (libsodium.a) and installs headers into a local
# prefix; the custom install: rule copies both into the staging tree.

NAME    := libsodium
VERSION := 1.0.20
CATEGORY:= security
LICENSE := ISC
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

SODIUM_PREFIX = $(FREELINX_BUILD_DIR)/deps/libsodium

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libsodium-$(VERSION)
PROJECT_BIN = $(SODIUM_PREFIX)/lib/libsodium.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(SODIUM_PREFIX)" \
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
	@$(INSTALL) -m 644 "$(SODIUM_PREFIX)/lib/libsodium.a" "$(STAGE_ROOT)/usr/lib/libsodium.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(SODIUM_PREFIX)/include/sodium.h" "$(STAGE_ROOT)/usr/include/sodium.h"
	@cp -r "$(SODIUM_PREFIX)/include/sodium" "$(STAGE_ROOT)/usr/include/"
	@printf '[FreeLinX/ports] staged libsodium\n'
