# FreeLinX/ports - www/links : Links 2.30 text-mode web browser.
#
# Links in text mode (no graphics drivers), TLS through OpenSSL, gzip
# content encoding through zlib.  GPL-2.0.  Built statically with the
# FreeLinX clang + LLD + musl toolchain.  Its configure predates
# VAR=value arguments, so the toolchain goes in through the environment.

NAME    := links
VERSION := 2.30
CATEGORY:= www
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), base/openssl, base/zlib
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= links
INSTALL_RELPATH:= usr/bin/links

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/links-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/links

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
ZLIB_PREFIX    = $(FREELINX_BUILD_DIR)/deps/zlib

PROJECT_CFG_CMDS = \
	CC="$(FREELINX_CC) --target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE)" \
	CPPFLAGS="-I$(OPENSSL_PREFIX)/include -I$(ZLIB_PREFIX)/include" CFLAGS="-O2" \
	LDFLAGS="-L$(OPENSSL_PREFIX)/lib64 -L$(ZLIB_PREFIX)/lib -fuse-ld=lld --rtlib=compiler-rt -static" \
	./configure --host=$(FREELINX_TRIPLE) --build=x86_64-pc-linux-gnu --prefix=/usr --sysconfdir=/etc \
		--without-x --without-fb --without-directfb --without-svgalib --without-pmshared \
		--without-atheos --without-haiku --without-gpm --without-libjpeg --without-libtiff \
		--without-librsvg --without-libwebp --without-libavif --without-lzma --without-bzip2 \
		--without-zstd --without-brotli --without-libevent \
		--with-ssl="$(OPENSSL_PREFIX)" --with-zlib

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
