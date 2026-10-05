# FreeLinX/ports - devel/libbsd : libbsd 0.12.2 (BSD compatibility library).
#
# Real upstream source:
#   https://libbsd.freedesktop.org/releases/libbsd-0.12.2.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Portable utility functions from BSD systems (strlcpy, arc4random,
# readpassphrase, ...).  Required by security/signify, which needs
# libbsd >= 0.11.  Upstream split the message digests out of libbsd, so
# this needs devel/libmd: its MD5/SHA-2 probes search libc or -lmd, and
# musl's libc has neither.  BSD-licensed.  Autoconf build; the release
# tarball ships a generated ./configure, so no autoreconf is needed.
#
# A library port.  It produces libbsd.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := libbsd
VERSION := 0.12.2
CATEGORY:= devel
LICENSE := BSD
DEPENDENCIES:= libmd
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), libmd (devel/libmd)
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

# The delivered archive and headers live here once installed; signify
# consumes them from this prefix (see security/signify).
INSTALL_RELPATH:= usr/lib

LIBBSD_PREFIX = $(FREELINX_BUILD_DIR)/deps/libbsd

# devel/libmd publishes build/deps/libmd, like every other library port
# in this tree.  The staging tree is where a port leaves what it installs
# into the rootfs, and a -I or -L into it is a -I or -L into the build
# host's /usr as well as ours -- so this port names the deps prefix.
PREFIX_libmd = $(FREELINX_BUILD_DIR)/deps/libmd

# libbsd is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libbsd-$(VERSION)
# The .libs/ archive is the real one.  libtool also writes a small ASCII
# "libtool library file" next to it, so only the .a counts.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libbsd.a) $(wildcard $(SRC_TREE)/src/libbsd.a))
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBBSD_PREFIX)" \
		--enable-static \
		--disable-shared \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_libmd)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_libmd)/lib"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

# A library stages itself: there is no single binary for install.mk to copy.
# `make install' above already installs the archive, the headers and the
# pkg-config files under $(LIBBSD_PREFIX); this recipe verifies every piece
# a consumer needs is really there.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(LIBBSD_PREFIX)/lib/libbsd.a" || { printf '[FreeLinX/ports][error] %s: libbsd.a was not installed under %s\n' "$(NAME)" "$(LIBBSD_PREFIX)" >&2; exit 1; }
	@test -d "$(LIBBSD_PREFIX)/include/bsd" || { printf '[FreeLinX/ports][error] %s: no headers at %s/include/bsd\n' "$(NAME)" "$(LIBBSD_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBBSD_PREFIX)/lib/pkgconfig/libbsd.pc" || { printf '[FreeLinX/ports][error] %s: libbsd.pc was not installed under %s\n' "$(NAME)" "$(LIBBSD_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBBSD_PREFIX)/lib/pkgconfig/libbsd-overlay.pc" || { printf '[FreeLinX/ports][error] %s: libbsd-overlay.pc was not installed under %s\n' "$(NAME)" "$(LIBBSD_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged libbsd into %s\n' "$(LIBBSD_PREFIX)"
