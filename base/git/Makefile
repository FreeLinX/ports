# FreeLinX/ports - base/git : Git 2.47.1 (upstream project port).
#
# Real upstream Git built with the FreeLinX clang + LLD + musl toolchain,
# fully static.  All optional/native dependencies are disabled:
#   NO_OPENSSL NO_CURL NO_EXPAT NO_GETTEXT NO_PERL NO_PYTHON NO_TCLTK
#   NO_PTHREADS NO_ICONV
# The only remaining required dependency is zlib (built by base/zlib into the
# build-time dependency prefix and linked statically via ZLIB_PATH).  The
# result is a genuine statically-linked, musl-based git with NO GNU runtime
# dependency (no glibc, no gettext/openssl/curl).
#
# The single `git` binary embeds the builtins (init/add/commit/status/log/...),
# so it is functionally complete for local version control without the
# `git-<builtin>` hardlinks.

NAME   := git
VERSION:= 2.47.1
CATEGORY:= base
LICENSE:= GPL-2.0
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl, zlib (base/zlib, statically linked)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= git
INSTALL_RELPATH:= bin/git

# Build-time dependency prefix produced by base/zlib.
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so this must resolve lazily.
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

# Git is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(ZLIB_PREFIX)/include
FLX_PROJECT_LDFLAGS  = -L$(ZLIB_PREFIX)/lib

# The FreeLinX toolchain's llvm-ar/llvm-ranlib require GLIBC_2.38 (host has
# 2.36) and cannot run here; archiving is libc-agnostic so host ar/ranlib
# package the musl .o files.  The final git link uses ONLY FreeLinX clang+LLD.
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/git
BUILD_BIN   = $(PROJECT_BIN)

# Build the single `git` binary (builtins are compiled in).  CC/AR/RANLIB and
# the FreeLinX toolchain flags are exported by project-port.mk; we add Git's
# dependency-disabling knobs and point it at the static zlib.
BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" git V=1 \
		CC="$(FREELINX_CC)" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(ZLIB_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(ZLIB_PREFIX)/lib" \
		ZLIB_PATH="$(ZLIB_PREFIX)" \
		NO_OPENSSL=1 NO_CURL=1 NO_EXPAT=1 NO_GETTEXT=1 \
		NO_PERL=1 NO_PYTHON=1 NO_TCLTK=1 NO_PTHREADS=1 NO_ICONV=1 \
		NO_REGEX=NeedsStartEnd
