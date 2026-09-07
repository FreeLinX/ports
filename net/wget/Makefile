# FreeLinX/ports - net/wget : wget 1.25.0 (network file retriever).
#
# GNU wget, HTTP/HTTPS/FTP downloader. Autoconf build, GPL-3.0 license.
# Links statically against OpenSSL for HTTPS support.

NAME    := wget
VERSION := 1.25.0
CATEGORY:= net
LICENSE := GPL-3.0
DEPENDENCIES:= openssl
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), OpenSSL (build/deps/openssl)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= wget
INSTALL_RELPATH:= usr/bin/wget

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
ZLIB_PREFIX    = $(FREELINX_BUILD_DIR)/deps/zlib

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/wget-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/wget
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=x86_64-pc-linux-gnu \
		--prefix=/usr \
		--enable-static \
		--disable-shared \
		--with-ssl=openssl \
		--with-openssl-crypto \
		--without-gpgme \
		--without-libpsl \
		--without-libidn2 \
		--disable-iri \
		--disable-pcre2 \
		OPENSSL_CFLAGS="-I$(OPENSSL_PREFIX)/include" \
		OPENSSL_LIBS="$(OPENSSL_PREFIX)/lib64/libssl.a $(OPENSSL_PREFIX)/lib64/libcrypto.a -ldl -lpthread" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(OPENSSL_PREFIX)/include -I$(ZLIB_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(ZLIB_PREFIX)/lib" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
