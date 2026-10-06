# FreeLinX/ports - graphics/libass : ASS/SSA subtitle renderer 0.17.5 (upstream port).
#
# Real upstream source:
#   https://github.com/libass/libass/releases/download/0.17.5/libass-0.17.5.tar.gz
# Built statically with FreeLinX clang + LLD + musl.
#
# Portable ASS/SSA subtitle renderer.  graphics/mpv needs it for styled
# subtitles.  ISC licensed.  Autoconf build; the release tarball ships a
# generated ./configure, so no autoreconf is needed.
#
# Dependencies, all found through pkg-config against the build/deps overlay:
# freetype2 (devel/freetype2), harfbuzz (devel/harfbuzz), fribidi
# (devel/fribidi) and fontconfig (x11/fontconfig).  HarfBuzz shapes
# complex-script subtitles, FriBidi orders right-to-left text, and fontconfig
# is the system font provider.
#
# A library port.  It produces libass.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := libass
VERSION := 0.17.5
CATEGORY:= graphics
LICENSE := ISC
DEPENDENCIES:= freetype2 harfbuzz fribidi fontconfig
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), freetype2 (devel/freetype2), harfbuzz (devel/harfbuzz), fribidi (devel/fribidi), fontconfig (x11/fontconfig)
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

# The delivered archive and headers live here once installed; mpv
# consumes them from this prefix (see graphics/mpv).
INSTALL_RELPATH:= usr/lib

LIBASS_PREFIX = $(FREELINX_BUILD_DIR)/deps/libass

# Every library port publishes build/deps/<name>, like every other library
# port in this tree.  The staging tree is where a port leaves what it
# installs into the rootfs, and a -I or -L into it is a -I or -L into the
# build host's /usr as well as ours -- so this port names the deps prefixes.
# freetype2.pc's own Cflags names include/freetype2, which is not where that
# port stages its headers, so its include directory is named explicitly here
# rather than trusted to pkg-config (see devel/harfbuzz).
PREFIX_freetype2 = $(FREELINX_BUILD_DIR)/deps/freetype2
PREFIX_harfbuzz = $(FREELINX_BUILD_DIR)/deps/harfbuzz
PREFIX_fribidi = $(FREELINX_BUILD_DIR)/deps/fribidi
PREFIX_fontconfig = $(FREELINX_BUILD_DIR)/deps/fontconfig

# libass is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_freetype2)/include -I$(PREFIX_harfbuzz)/include -I$(PREFIX_fribidi)/include -I$(PREFIX_fontconfig)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_freetype2)/lib -L$(PREFIX_harfbuzz)/lib -L$(PREFIX_fribidi)/lib -L$(PREFIX_fontconfig)/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libass-$(VERSION)
# The .libs/ archive is the real one.  libtool also writes a small ASCII
# "libtool library file" next to it, so only the .a counts.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/libass/.libs/libass.a) $(wildcard $(SRC_TREE)/libass/libass.a))
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBASS_PREFIX)" \
		--enable-static \
		--disable-shared \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_freetype2)/include -I$(PREFIX_harfbuzz)/include -I$(PREFIX_fribidi)/include -I$(PREFIX_fontconfig)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_freetype2)/lib -L$(PREFIX_harfbuzz)/lib -L$(PREFIX_fribidi)/lib -L$(PREFIX_fontconfig)/lib"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

# A library stages itself: there is no single binary for install.mk to copy.
# `make install' above already installs the archive, the headers and the
# pkg-config files under $(LIBASS_PREFIX); this recipe verifies every piece
# a consumer needs is really there.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(LIBASS_PREFIX)/lib/libass.a" || { printf '[FreeLinX/ports][error] %s: libass.a was not installed under %s\n' "$(NAME)" "$(LIBASS_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBASS_PREFIX)/include/ass/ass.h" || { printf '[FreeLinX/ports][error] %s: no headers at %s/include/ass\n' "$(NAME)" "$(LIBASS_PREFIX)" >&2; exit 1; }
	@test -f "$(LIBASS_PREFIX)/lib/pkgconfig/libass.pc" || { printf '[FreeLinX/ports][error] %s: libass.pc was not installed under %s\n' "$(NAME)" "$(LIBASS_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged libass into %s\n' "$(LIBASS_PREFIX)"
