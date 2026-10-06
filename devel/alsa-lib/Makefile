# FreeLinX/ports - devel/alsa-lib : ALSA library 1.2.16 (upstream project port).
#
# Real upstream source:
#   https://www.alsa-project.org/files/pub/lib/alsa-lib-1.2.16.tar.bz2
# Built statically with FreeLinX clang + LLD + musl.
#
# The Advanced Linux Sound Architecture userspace library.  audio/sndio needs
# it on Linux: its sndiod backend probes for <alsa/asoundlib.h>, and without
# the headers the Linux backend is silently dropped from the build.
#
# LGPL-2.1.  Autoconf build; the release tarball ships a generated
# ./configure, so no autoreconf is needed.
#
# A library port.  It produces libasound.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := alsa-lib
VERSION := 1.2.16
CATEGORY:= devel
LICENSE := LGPL-2.1
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

# The delivered archive and headers live here once installed; sndio
# consumes them from this prefix (see audio/sndio).
INSTALL_RELPATH:= usr/lib

ALSA_PREFIX = $(FREELINX_BUILD_DIR)/deps/alsa-lib

# alsa-lib is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/alsa-lib-$(VERSION)
# The .libs/ archive is the real one.  libtool also writes a small ASCII
# "libtool library file" next to it, so only the .a counts.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libasound.a) $(wildcard $(SRC_TREE)/src/libasound.a))
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(ALSA_PREFIX)" \
		--enable-static \
		--disable-shared \
		--without-debug \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

# A library stages itself: there is no single binary for install.mk to copy.
# `make install' above already installs the archive, the headers and the
# pkg-config files under $(ALSA_PREFIX); this recipe verifies every piece
# a consumer needs is really there.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(ALSA_PREFIX)/lib/libasound.a" || { printf '[FreeLinX/ports][error] %s: libasound.a was not installed under %s\n' "$(NAME)" "$(ALSA_PREFIX)" >&2; exit 1; }
	@test -f "$(ALSA_PREFIX)/include/alsa/asoundlib.h" || { printf '[FreeLinX/ports][error] %s: no headers at %s/include/alsa\n' "$(NAME)" "$(ALSA_PREFIX)" >&2; exit 1; }
	@test -f "$(ALSA_PREFIX)/lib/pkgconfig/alsa.pc" || { printf '[FreeLinX/ports][error] %s: alsa.pc was not installed under %s\n' "$(NAME)" "$(ALSA_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged alsa-lib into %s\n' "$(ALSA_PREFIX)"
