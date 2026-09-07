# FreeLinX/ports - base/mandoc : mandoc 1.14.6 (man page formatter/viewer).
#
# BSD-2-Clause, from OpenBSD.  Replaces GNU groff + man.
# Built with FreeLinX clang + LLD + musl, static.
#
# NOTES (2026-09, reproduced by hand):
#   - mandoc's configure does NOT honour the CC/CFLAGS/LDFLAGS it is handed
#     (Makefile.local ends up as `CC = cc` with host CFLAGS), so the framework
#     "make all" path links host binaries.  The proven cross build is done
#     manually, per the recipe below; keep it in sync if touching this port.
#
#   - makewhatis/mandocdb need fts(3), and the FreeLinX musl sysroot has
#     neither <fts.h> nor fts symbols in libc.a.  Fix: set HAVE_FTS to 0 in
#     config.h after configure (mandocdb.c then switches to the bundled
#     compat_fts.c) and `ar rs libmandoc.a compat_fts.o`.
#
#   - mandoc unconditionally links zlib for compressed pages; the musl static
#     zlib lives in build/deps/zlib (see ../zlib).  Link with its archives.
#
# Cross build recipe:
#   cd $(FREELINX_BUILD_DIR)/work/mandoc/mandoc-1.14.6
#   TRIPLE=$(FREELINX_TRIPLE); SYS=$(FREELINX_SYSROOT)
#   D=/home/devuan/FreeLinX/ports/build/deps/zlib
#   CC=$(FREELINX_TOOLCHAIN_BIN)/clang
#   ./configure --prefix=/ --host=$TRIPLE \
#       CC="$CC" CFLAGS="--target=$TRIPLE --sysroot=$SYS -O2" \
#       LDFLAGS="--target=$TRIPLE --sysroot=$SYS --rtlib=compiler-rt -static -fuse-ld=lld"
#   sed -i 's/#define HAVE_FTS 1/#define HAVE_FTS 0/' config.h
#   $CC --target=$TRIPLE --sysroot=$SYS -O2 -I. -I$SYS/include -c compat_fts.c -o compat_fts.o
#   ar rs libmandoc.a compat_fts.o
#   make CC=$CC CFLAGS="--target=$TRIPLE --sysroot=$SYS -O2 -I. -I$D/include" \
#        LDFLAGS="--target=$TRIPLE --sysroot=$SYS --rtlib=compiler-rt -static -fuse-ld=lld" \
#        LDADD="$D/lib/libz.a"
#   ln -sf mandoc apropos whatis mandocdb makewhatis nroff tbl eqn
# Delivered tools: mandoc, man, apropos, whatis, mandocdb, makewhatis,
# nroff, tbl, eqn, demandoc, soelim (all but demandoc/soelim are argv[0]
# aliases of mandoc).

NAME   := mandoc
VERSION:= 1.14.6
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mandoc
INSTALL_RELPATH:= bin/mandoc

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/mandoc-1.14.6
PROJECT_BIN = $(SRC_TREE)/mandoc
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--host=$(FREELINX_TRIPLE) \
		--enable-static \
		--disable-shared \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"