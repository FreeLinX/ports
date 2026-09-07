# FreeLinX/ports - net/socat : socat 1.8.0.2 (multipurpose relay).
#
# Bidirectional data relay between two addresses. Autoconf build.
# GPL-2.0-only license. Simple static build.

NAME    := socat
VERSION := 1.8.0.2
CATEGORY:= net
LICENSE := GPL-2.0-only
DEPENDENCIES:= openssl
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), OpenSSL (build/deps/openssl)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= socat
INSTALL_RELPATH:= usr/bin/socat

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/socat-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/socat
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		--with-openssl \
		OPENSSL_CFLAGS="-I$(OPENSSL_PREFIX)/include" \
		OPENSSL_LIBS="$(OPENSSL_PREFIX)/lib64/libssl.a $(OPENSSL_PREFIX)/lib64/libcrypto.a -ldl -lpthread" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(OPENSSL_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(OPENSSL_PREFIX)/lib64" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	$(MAKE) -j"$(FREELINX_JOBS)"
