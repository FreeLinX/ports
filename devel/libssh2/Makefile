# FreeLinX/ports - devel/libssh2 : libssh2 1.11.1 (SSH2 protocol).
#
# Client-side SSH2 protocol library. BSD-3-Clause license. Autoconf build,
# static only. Depends on OpenSSL for crypto.

NAME    := libssh2
VERSION := 1.11.1
CATEGORY:= devel
LICENSE := BSD-3-Clause
DEPENDENCIES:= openssl
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), OpenSSL (base/openssl)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= usr/lib

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
LIBSSH2_PREFIX = $(FREELINX_BUILD_DIR)/deps/libssh2

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libssh2-$(VERSION)
PROJECT_BIN = $(LIBSSH2_PREFIX)/lib/libssh2.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBSSH2_PREFIX)" \
		--enable-static \
		--disable-shared \
		--with-crypto=openssl \
		--with-libssl-prefix="$(OPENSSL_PREFIX)" \
		--without-libgcrypt \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(OPENSSL_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(OPENSSL_PREFIX)/lib64"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libssh2.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(LIBSSH2_PREFIX)/include/libssh2.h" "$(STAGE_ROOT)/usr/include/libssh2.h"
	@printf '[FreeLinX/ports] staged libssh2\n'
