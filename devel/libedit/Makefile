# FreeLinX/ports - devel/libedit : libedit 20260512-3.1 (BSD command-line
# editing library, readline replacement).
#
# Real upstream source:
#   https://www.thrysoee.dk/editline/libedit-20260512-3.1.tar.gz
# Built statically with FreeLinX clang + LLD + musl.
#
# NOTE: upstream publishes no checksum for its tarballs; the SHA256 in
# distinfo was computed locally after download, not verified against a
# published value.
#
# Autotools port (the release tarball ships a generated ./configure, so no
# autoreconf is needed), in the devel/libmd + devel/libbsd style: static
# archive, public headers and a pkg-config file staged into the build/deps
# overlay for the next port in the chain (python).  Wide-char/UTF-8 needs no
# flag: upstream builds it unconditionally since 2016 (--enable-widec is a
# deprecated no-op), so history/completion work out of the box and the only
# narrowing needed is --disable-examples.
#
# Curses comes from the tree's devel/netbsd-curses port: libedit's configure
# probes -lncurses/-lcurses/-ltermcap/-ltinfo for tgetent, and every one of
# those names exists under $(CURSES_PREFIX)/lib, so -I/-L at that prefix is
# all the detection needs (same shape as base/less and sysutils/tmux).
#
# A library port.  It produces libedit.a into the build/deps overlay, and
# the next port in the dependency line names that prefix.  It stages nothing
# into the rootfs and ships no xpkg: a .a is build-time only, and shipping
# one into /usr/lib would be dead weight in a static image.

NAME    := libedit
VERSION := 20260512-3.1
CATEGORY:= devel
LICENSE := BSD-3-Clause
DEPENDENCIES:= netbsd-curses
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), netbsd-curses (devel/netbsd-curses)
# DEPS_ONLY: this port stages nothing into the overlay or the rootfs.
# It exists to put headers and a .a in build/deps for the next port
# in the chain.  install.sh needs to be told, or it expects a staged
# binary that a library never has.
DEPS_ONLY := yes
# LIBRARY_PORT: no single staged binary, so the framework's staging
# rules are skipped and the port stages its own .a and headers.
# Declared BEFORE the include, because make reads prerequisite lists
# at parse time -- see the STAGE_DEP note in mk/install.mk.
LIBRARY_PORT := yes
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

# The delivered archive and headers live here once installed; python
# consumes them from this prefix (see devel/libbsd for the pattern).
INSTALL_RELPATH:= usr/lib

LIBEDIT_PREFIX = $(FREELINX_BUILD_DIR)/deps/libedit

# devel/netbsd-curses publishes build/deps/netbsd-curses, like every other
# library port in this tree.  The staging tree is where a port leaves what
# it installs into the rootfs, and a -I or -L into it is a -I or -L into
# the build host's /usr as well as ours -- so this port names the deps
# prefix.
CURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/netbsd-curses

# libedit is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libedit-$(VERSION)
# The .libs/ archive is the real one.  libtool also writes a small ASCII
# "libtool library file" next to it, so only the .a counts.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libedit.a) $(wildcard $(SRC_TREE)/src/libedit.a))
BUILD_BIN   = $(PROJECT_LIB)

# musl's wchar_t is ISO 10646 UCS-4 (32-bit, __STDC_UTF_32__=1), and musl
# says so in its own include/stdc-predef.h (__STDC_ISO_10646__ 201206L) --
# but clang, unlike GCC, does not pre-include stdc-predef.h, so libedit's
# src/chartype.h errors out with "wchar_t must store ISO 10646 characters".
# State the same value musl declares on the command line, for configure
# tests (CPPFLAGS) and the build (CFLAGS) alike.
LIBEDIT_ISO10646 = -D__STDC_ISO_10646__=201206L

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBEDIT_PREFIX)" \
		--enable-static \
		--disable-shared \
		--disable-examples \
		CC="$(FREELINX_CC)" \
		CPPFLAGS="-I$(CURSES_PREFIX)/include $(LIBEDIT_ISO10646)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(CURSES_PREFIX)/include $(LIBEDIT_ISO10646)" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(CURSES_PREFIX)/lib"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

# A library stages itself: there is no single binary for install.mk to copy.
# `make install' above already installs the archive, the headers and the
# pkg-config file under $(LIBEDIT_PREFIX); this recipe verifies every piece
# a consumer needs is really there.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(LIBEDIT_PREFIX)/lib/libedit.a" || { printf '[FreeLinX/ports][error] %s: libedit.a was not installed under %s\n' "$(NAME)" "$(LIBEDIT_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBEDIT_PREFIX)/include/histedit.h" || { printf '[FreeLinX/ports][error] %s: no histedit.h at %s/include\n' "$(NAME)" "$(LIBEDIT_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBEDIT_PREFIX)/include/editline/readline.h" || { printf '[FreeLinX/ports][error] %s: no editline/readline.h at %s/include\n' "$(NAME)" "$(LIBEDIT_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBEDIT_PREFIX)/lib/pkgconfig/libedit.pc" || { printf '[FreeLinX/ports][error] %s: libedit.pc was not installed under %s\n' "$(NAME)" "$(LIBEDIT_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged libedit into %s\n' "$(LIBEDIT_PREFIX)"
