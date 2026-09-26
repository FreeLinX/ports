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

# lynx 2.9.2 predates musl.  Its configure asks AC_TYPE_PID_T and three
# friends whether those types are pre-defined, and when cross-compiling it
# cannot answer, so it *defines* them in the generated lynx_cfg.h:
#
#	#define pid_t int			AC_TYPE_PID_T
#	#define mode_t int			AC_TYPE_MODE_T
#	#define intptr_t long			AC_CHECK_TYPE
#	#define socklen_t int			CF_CHECK_TYPE
#
# Those four are all libc builtins, so musl's <sys/types.h> already declares
# them - and "#define pid_t int" turns musl's own "typedef int pid_t;" into
# "typedef int int;", which is a hard error:
#
#	bits/alltypes.h:235:13: error: cannot combine with previous 'int' declaration specifier
#
# The fix belongs in configure, not in a post-hoc sed on a generated file:
# answer the four questions it could not answer.  ac_cv_type_*=yes tells each
# macro the type is a builtin, so the #define is never emitted.  The same
# question is asked with three different macros (AC_TYPE_*, AC_CHECK_TYPE and
# CF_CHECK_TYPE) but the cache variable is ac_cv_type_<name> for all three.
PROJECT_CFG_CMDS = \
	ac_cv_type_intptr_t=yes \
	ac_cv_type_mode_t=yes \
	ac_cv_type_pid_t=yes \
	ac_cv_type_socklen_t=yes \
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
		--disable-color-style \
		--disable-option-checking \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)" \
		CPPFLAGS="-DHAVE_PUTENV=1 -DNCURSES_WIDECHAR=1 $(FLX_NBSYS_FLAGS)" \
		LIBS="-lncursesw -lz -lssl -lcrypto"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"