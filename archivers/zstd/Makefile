# FreeLinX/ports - archivers/zstd : zstd 1.5.7 (Zstandard compression).
#
# Facebook's fast lossless compression algorithm. Provides zstd, zstdcat,
# unzstd, zstdmt for .zst compression. Multi-threaded support.

NAME    := zstd
VERSION := 1.5.7
CATEGORY:= archivers
LICENSE := BSD-3-Clause+GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= zstd
INSTALL_RELPATH:= usr/bin/zstd

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/zstd-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/programs/zstd
BUILD_BIN   = $(PROJECT_BIN)

# install-pc install-static install-includes, and not `install'.  lib/Makefile has
#
#   install: install-pc install-static install-shared install-includes
#
# and install-shared builds libzstd.so, which does not link here:
#
#   ld.lld: error: duplicate symbol: _init
#   ld.lld: error: duplicate symbol: _fini
#
# because a shared object on this target pulls in the host's crtbeginS.o and
# crtendS.o, which define _init and _fini.  x11/xf86-video-fbdev hits the same
# thing for the same reason.  Nothing in this tree links zstd dynamically --
# every binary here is static and the rootfs carries no shared library but musl's
# libc.so -- so the shared object would have no consumer.  install-pc and
# install-static come first in the prerequisite list, which is why libzstd.a and
# libzstd.pc used to be staged and the build still reported a failure.
#
# install-pc install-static install-includes, and not `install'.  lib/Makefile has
#
#   install: install-pc install-static install-shared install-includes
#
# and install-shared builds libzstd.so, which does not link here:
#
#   ld.lld: error: duplicate symbol: _init
#   ld.lld: error: duplicate symbol: _fini
#
# because a shared object on this target pulls in the host's crtbeginS.o and
# crtendS.o, which define _init and _fini.  x11/xf86-video-fbdev hits the same
# thing for the same reason.  Nothing in this tree links zstd dynamically --
# every binary here is static and the rootfs carries no shared library but musl's
# libc.so -- so the shared object would have no consumer.  install-pc and
# install-static come first in the prerequisite list, which is why libzstd.a and
# libzstd.pc used to be staged and the build still reported a failure.
#
# -nostdlib and musl's own crt objects in the programs LDFLAGS, in the same
# order mk uses: without them the zstd binary picks up the host's crt1.o and
#
#   ld.lld: error: duplicate symbol: _start
#   ld.lld: error: duplicate symbol: _start_c
#
# because musl's crt1.o is on the link line too.
#
# zstd uses make-based build system with cross-compilation support
#
# prefix is $(DEPS)/zstd/usr and not /usr, because zstd's lib/Makefile writes
# libzstd.pc from prefix and the staged file then says
#
#   prefix=/usr
#
# A pkg-config file with prefix=/usr has its "Cflags: -I/usr/include" dropped as
# a system path, so the module contributes no include flags at all and a consumer
# compiles against whatever the host has.
#
# PREFIX, EXEC_PREFIX, INCLUDEDIR, LIBDIR, BINDIR and MANDIR are named
# explicitly rather than left to their defaults, because libzstd.pc.in writes them
# in resolved form:
#
#   prefix=@PREFIX@
#   includedir=@INCLUDEDIR@
#   libdir=@LIBDIR@
#
# and DESTDIR does not appear in any of them.  Naming them is also what puts the
# files where the port expects them: with DESTDIR=$(DEPS)/zstd and prefix=usr the
# archive landed at
#
#   deps/zstd/usr/lib/libzstd.a
#
# but so did the pkg-config file, with a relative prefix, because zstd's
# install-pc uses $(DESTDIR)$(PKGCONFIGDIR) and PKGCONFIGDIR defaults to
# $(PREFIX)/libdata/pkgconfig on some systems.  No DESTDIR at all is simpler and
# has one answer instead of two.
ZSTD_STAGE = $(FREELINX_BUILD_DIR)/deps/zstd
ZSTD_LIBZSTD_PC = $(ZSTD_STAGE)/usr/lib/pkgconfig/libzstd.pc

# prefix=/usr with DESTDIR, which is the invocation zstd's own make expects:
# install-pc uses $(DESTDIR)$(PKGCONFIGDIR) and PKGCONFIGDIR defaults to
# $(LIBDIR)/pkgconfig, so an absolute prefix there duplicates DESTDIR and every
# file lands under deps/zstd/home/kanan/... .
#
# install-pc install-static install-includes, and not `install'.  lib/Makefile has
#
#   install: install-pc install-static install-shared install-includes
#
# and install-shared builds libzstd.so, which does not link here:
#
#   ld.lld: error: duplicate symbol: _init
#   ld.lld: error: duplicate symbol: _fini
#
# because a shared object on this target pulls in the host's crtbeginS.o and
# crtendS.o.  x11/xf86-video-fbdev hits the same thing for the same reason.
# Nothing in this tree links zstd dynamically -- every binary here is static and
# the rootfs carries no shared library but musl's libc.so -- so the shared
# object would have no consumer.  install-pc and install-static come first in the
# prerequisite list, which is why libzstd.a and libzstd.pc used to be staged while
# the build still reported a failure.
#
# -nostdlib and musl's own crt objects in the programs LDFLAGS, in the order mk
# uses: without them the zstd binary picks up the host's crt1.o as well as
# musl's and stops at
#
#   ld.lld: error: duplicate symbol: _start
#   ld.lld: error: duplicate symbol: _start_c
PROJECT_CFG_CMDS = \
	$(MAKE) -C $(SRC_TREE)/lib \
		DESTDIR="$(ZSTD_STAGE)" \
		prefix=/usr \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2" \
		AR="$(FREELINX_TOOLCHAIN_BIN)/llvm-ar" \
		RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
		install-pc install-static install-includes

# The staged libzstd.pc says prefix=/usr, because that is the prefix zstd's make
# was given, and a pkg-config module with prefix=/usr has its
# "Cflags: -I/usr/include" dropped as a system path.  A consumer then gets no
# include flags at all and compiles against whatever the host has.  The file uses
# ${prefix} and ${exec_prefix} for its directories, so the prefix line is the
# only thing that has to change:
#
#   prefix=/usr
#   includedir=${prefix}/include
#   libdir=${exec_prefix}/lib
BUILD_CMDS = \
	$(MAKE) -C $(SRC_TREE)/programs \
		DESTDIR="$(ZSTD_STAGE)" \
		prefix=/usr \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -nostdlib $(FLX_CRT) -lc $(FLX_CRT_END) -static -fuse-ld=lld --rtlib=compiler-rt" \
		AR="$(FREELINX_TOOLCHAIN_BIN)/llvm-ar" \
		RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
		install && \
	sed 's|^prefix=/usr$$|prefix=$(ZSTD_STAGE)/usr|' "$(ZSTD_LIBZSTD_PC)" > "$(ZSTD_LIBZSTD_PC).new" && \
	mv "$(ZSTD_LIBZSTD_PC).new" "$(ZSTD_LIBZSTD_PC)" && \
	grep -q '^prefix=$(ZSTD_STAGE)/usr$$' "$(ZSTD_LIBZSTD_PC)"
