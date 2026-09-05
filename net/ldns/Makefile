# FreeLinX/ports - net/ldns : ldns library + drill DNS tool (1.8.3).
#
# DNS library and drill DNSSEC query tool.
# BSD license. Autotools build.
# Note: patch keys.c to disable ENGINE API (removed in OpenSSL 3.x).

NAME    := ldns
VERSION := 1.8.3
CATEGORY:= net
LICENSE := BSD-3-Clause
DEPENDENCIES:= openssl
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= drill
INSTALL_RELPATH:= usr/bin/drill

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/ldns-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/drill/.libs/drill

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
NATIVE_CC = /usr/bin/cc

PROJECT_CFG_CMDS = \
	cd "$(SRC_TREE)" && \
	sed -i 's/k->_key.key = ENGINE_load_private_key(e, key_id, UI_OpenSSL(), NULL);/k->_key.key = NULL;/' keys.c && \
	./configure \
		--host=x86_64-pc-linux-gnu \
		--prefix=/usr \
		--with-drill \
		--with-ssl="$(OPENSSL_PREFIX)" \
		--disable-dane \
		--without-pyldns \
		--without-examples \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(OPENSSL_PREFIX)/include -fPIC" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -L$(OPENSSL_PREFIX)/lib64 -fuse-ld=lld --rtlib=compiler-rt" \
		AR="$(FREELINX_AR)" \
		RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	make -j"$(FREELINX_JOBS)" all
