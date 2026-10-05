# FreeLinX/ports - devel/libmd : libmd 1.2.0 (BSD message digest library).
#
# Real upstream source:
#   https://libbsd.freedesktop.org/releases/libmd-1.2.0.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Message digest functions from BSD systems (MD2/MD4/MD5/SHA1/SHA2/RIPEMD160).
# Upstream split the digests out of libbsd, so devel/libbsd 0.12.x cannot
# even configure without this: its MD5/SHA-2 probes search libc or -lmd,
# and musl's libc has neither.  BSD-licensed.  Autoconf build; the release
# tarball ships a generated ./configure, so no autoreconf is needed.
#
# A library port.  It produces libmd.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := libmd
VERSION := 1.2.0
CATEGORY:= devel
LICENSE := BSD
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
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

# The delivered archive and headers live here once installed; libbsd
# consumes them from this prefix (see devel/libbsd).
INSTALL_RELPATH:= usr/lib

LIBMD_PREFIX = $(FREELINX_BUILD_DIR)/deps/libmd

# libmd is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libmd-$(VERSION)
# The .libs/ archive is the real one.  libtool also writes a small ASCII
# "libtool library file" next to it, so only the .a counts.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libmd.a) $(wildcard $(SRC_TREE)/src/libmd.a))
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBMD_PREFIX)" \
		--enable-static \
		--disable-shared \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

# A library stages itself: there is no single binary for install.mk to copy.
# `make install' above already installs the archive, the headers and the
# pkg-config file under $(LIBMD_PREFIX); this recipe verifies every piece
# a consumer needs is really there.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(LIBMD_PREFIX)/lib/libmd.a" || { printf '[FreeLinX/ports][error] %s: libmd.a was not installed under %s\n' "$(NAME)" "$(LIBMD_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBMD_PREFIX)/include/md5.h" || { printf '[FreeLinX/ports][error] %s: no digest headers at %s/include\n' "$(NAME)" "$(LIBMD_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBMD_PREFIX)/lib/pkgconfig/libmd.pc" || { printf '[FreeLinX/ports][error] %s: libmd.pc was not installed under %s\n' "$(NAME)" "$(LIBMD_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged libmd into %s\n' "$(LIBMD_PREFIX)"
