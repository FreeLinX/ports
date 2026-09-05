# FreeLinX/ports - net/curl : curl 8.12.1 (HTTP client).
#
# The de facto standard HTTP client for command line. Supports HTTP, HTTPS,
# FTP, SFTP, SCP, and many other protocols. Depends on OpenSSL and zlib
# (both already ported as build dependencies).

NAME    := curl
VERSION := 8.12.1
CATEGORY:= net
LICENSE := MIT
DEPENDENCIES:= openssl zlib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), OpenSSL (base/openssl), zlib (base/zlib)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= curl
INSTALL_RELPATH:= usr/bin/curl

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/curl-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/curl
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--enable-static \
		--disable-shared \
		--with-openssl="$(OPENSSL_PREFIX)" \
		--with-zlib="$(ZLIB_PREFIX)" \
		--disable-ldap \
		--disable-ldaps \
		--disable-rtsp \
		--disable-dict \
		--disable-telnet \
		--disable-pop3 \
		--disable-imap \
		--disable-smb \
		--disable-smtp \
		--disable-gopher \
		--disable-mqtt \
		--disable-manual \
		--disable-sspi \
		--disable-ntlm \
		--without-libidn2 \
		--without-librtmp \
		--without-nghttp2 \
		--without-brotli \
		--without-zstd \
		--without-libpsl \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(OPENSSL_PREFIX)/include -I$(ZLIB_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(OPENSSL_PREFIX)/lib64 -L$(ZLIB_PREFIX)/lib" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
