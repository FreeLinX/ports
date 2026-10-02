# FreeLinX/ports - www/w3m : w3m text-based web browser (0.5.3+git20230121).
#
# Text-mode web browser with TLS.  The maintained w3m (tats/w3m, the one
# Debian and the BSDs ship) - the 2011 sourceforge 0.5.3 is abandoned.
# w3m license.  Autotools build, static.
# Requires: libgc (devel/libgc), NetBSD curses, OpenSSL, zlib.
#
# Cross-compiling needs w3m's table generator (mktable) to run on the build
# host.  It is linked -static like everything else here, so the musl binary
# runs on the host directly: no host C compiler is involved.

NAME    := w3m
VERSION := 0.5.3+git20230121
CATEGORY:= www
LICENSE := w3m
DEPENDENCIES:= libgc netbsd-curses openssl
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), devel/libgc, devel/netbsd-curses, base/openssl, base/zlib
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= w3m
INSTALL_RELPATH:= bin/w3m

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/w3m-0.5.3-git20230121
PROJECT_BIN = $(SRC_TREE)/w3m

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/netbsd-curses
LIBGC_TREE     = $(FREELINX_BUILD_DIR)/work/libgc/bdwgc-8.2.8
ZLIB_PREFIX    = $(FREELINX_BUILD_DIR)/deps/zlib

W3M_CC = $(FREELINX_CC) --target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE)
W3M_CFLAGS = -O2 -I$(NCURSES_PREFIX)/include -I$(LIBGC_TREE)/include \
	-I$(OPENSSL_PREFIX)/include -I$(ZLIB_PREFIX)/include \
	-Wno-int-conversion -Wno-incompatible-pointer-types \
	-Wno-implicit-function-declaration -Wno-implicit-int
W3M_LDFLAGS = -L$(LIBGC_TREE)/.libs -L$(NCURSES_PREFIX)/lib -L$(OPENSSL_PREFIX)/lib64 \
	-L$(ZLIB_PREFIX)/lib -fuse-ld=lld --rtlib=compiler-rt -static

PROJECT_CFG_CMDS = \
	test -f "$(LIBGC_TREE)/.libs/libgc.a" || { echo "build devel/libgc first" >&2; exit 1; }; \
	./configure --host=$(FREELINX_TRIPLE) --build=x86_64-pc-linux-gnu --prefix=/usr --sysconfdir=/etc \
		--disable-image --disable-xface --disable-nls \
		--with-ssl="$(OPENSSL_PREFIX)" --with-gc="$(LIBGC_TREE)" \
		--with-termlib="curses terminfo" ac_cv_func_setpgrp_void=yes \
		CC="$(W3M_CC)" CXX="$(W3M_CC)" \
		CFLAGS="$(W3M_CFLAGS)" LDFLAGS="$(W3M_LDFLAGS)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" w3m
