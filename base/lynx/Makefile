# FreeLinX/ports - base/lynx : Lynx 2.9.2 (text-mode web browser).
#
# Real upstream Lynx (invisible-island.net, lynx2.9.2.tar.bz2) built with the
# FreeLinX clang + LLD + musl toolchain, fully static.  Dependencies:
#   - ncurses (base/ncurses, static, wide-char) for the terminal UI
#   - zlib (base/zlib) for gzip content-encoding
#   - openssl (base/openssl) for HTTPS
# Lynx is a genuine text-mode browser with NO GNU runtime dependency.

NAME   := lynx
VERSION:= 2.9.2
CATEGORY:= base
LICENSE:= ISC-style (lynx2.8.8rel.2)
DEPENDENCIES:= ncurses zlib openssl
BUILD_DEPENDENCIES:= musl, ncurses, zlib, openssl
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= lynx
INSTALL_RELPATH:= bin/lynx

# Build-time dependency prefixes (lazy = so they bind after project-port.mk
# sets FREELINX_BUILD_DIR).
NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/ncurses
ZLIB_PREFIX    = $(FREELINX_BUILD_DIR)/deps/zlib
OSSL_PREFIX    = $(FREELINX_BUILD_DIR)/deps/openssl

FLX_PROJECT_LIBS :=

# Link-only flags folded into CFLAGS so lynx's configure feature-test link
# works (same reason as net/dhcpcd).  No include prefix here: header dirs are
# expanded lazily via CPPFLAGS (below) so the prefix binds correctly.
FLX_PROJECT_CFLAGS := --rtlib=compiler-rt -fuse-ld=lld -static -O2

FLX_PROJECT_CPPFLAGS = -I$(NCURSES_PREFIX)/include -I$(NCURSES_PREFIX)/include/ncursesw \
	-I$(ZLIB_PREFIX)/include -I$(OSSL_PREFIX)/include
FLX_PROJECT_LDFLAGS  = -L$(NCURSES_PREFIX)/lib -L$(ZLIB_PREFIX)/lib -L$(OSSL_PREFIX)/lib64

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/lynx
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--host=x86_64-linux-musl \
		--build=x86_64-linux-gnu \
		--with-screen=ncurses \
		--with-curses-dir="$(NCURSES_PREFIX)" \
		--without-x \
		--with-zlib \
		--with-ssl \
		--disable-locale-charset --disable-nls \
		--without-mswin --without-cygwin \
		--enable-nested-tables \
		--disable-option-checking \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)" \
		LIBS="-lncursesw -lz -lssl -lcrypto"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"