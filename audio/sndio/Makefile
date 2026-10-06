# FreeLinX/ports - audio/sndio : sndio audio framework 1.10.0 (upstream port).
#
# Real upstream source:
#   https://sndio.org/sndio-1.10.0.tar.gz
# Built statically with FreeLinX clang + LLD + musl.
#
# OpenBSD's sndio sound server and client library, portable to Linux via its
# ALSA backend.  graphics/mpv plays audio through it.  ISC licensed.
#
# Upstream configure (not autoconf: a small Bourne script writing Makefiles
# from Makefile.in).  It takes --prefix plus CC=/CFLAGS=/LDFLAGS=/AR=
# variables, which are spliced into every generated Makefile -- so the
# FreeLinX toolchain and the ALSA prefix reach the build that way rather than
# through the environment.
#
# Linux backend needs devel/alsa-lib: configure enables ALSA on Linux by
# default, which compiles sio_alsa.c/mio_alsa.c against <alsa/asoundlib.h>
# and links -lasound.  Without the headers the backend is silently dropped.
#
# --enable-static builds libsndio.a instead of the .so: every link in this
# tree is static.  The daemon (sndiod) and the tools (sndioctl, aucat,
# midicat) are built too and installed under the deps prefix beside the
# library.
#
# A library port.  It produces libsndio.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: the archive is build-time only.

NAME    := sndio
VERSION := 1.10.0
CATEGORY:= audio
LICENSE := ISC
DEPENDENCIES:= alsa-lib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), alsa-lib (devel/alsa-lib)
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

SNDIO_PREFIX = $(FREELINX_BUILD_DIR)/deps/sndio

# devel/alsa-lib publishes build/deps/alsa-lib, like every other library
# port in this tree.  The staging tree is where a port leaves what it
# installs into the rootfs, and a -I or -L into it is a -I or -L into the
# build host's /usr as well as ours -- so this port names the deps prefix.
PREFIX_alsa = $(FREELINX_BUILD_DIR)/deps/alsa-lib

# sndio is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/sndio-$(VERSION)
PROJECT_LIB = $(SRC_TREE)/libsndio/libsndio.a
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix="$(SNDIO_PREFIX)" \
		--enable-static \
		--enable-alsa \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_alsa)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_alsa)/lib" \
		AR="$(FREELINX_AR)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

# A library stages itself: there is no single binary for install.mk to copy.
# `make install' above already installs the archive, the header, the tools
# and the pkg-config file under $(SNDIO_PREFIX); this recipe verifies every
# piece a consumer needs is really there.
#
# Requires.private: configure writes Libs: -lsndio with no mention of ALSA,
# and the static archive references asound symbols, so a consumer linking
# only `pkg-config --static --libs sndio' stops at undefined sndio/ALSA
# references with nothing in the log about ALSA.  Appending the private
# requirement makes the dependency explicit where mpv's configure looks.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(SNDIO_PREFIX)/lib/libsndio.a" || { printf '[FreeLinX/ports][error] %s: libsndio.a was not installed under %s\n' "$(NAME)" "$(SNDIO_PREFIX)" >&2; exit 1; }
	@test -f "$(SNDIO_PREFIX)/include/sndio.h" || { printf '[FreeLinX/ports][error] %s: no header at %s/include/sndio.h\n' "$(NAME)" "$(SNDIO_PREFIX)" >&2; exit 1; }
	@test -f "$(SNDIO_PREFIX)/lib/pkgconfig/sndio.pc" || { printf '[FreeLinX/ports][error] %s: sndio.pc was not installed under %s\n' "$(NAME)" "$(SNDIO_PREFIX)" >&2; exit 1; }
	@grep -q '^Requires.private: alsa' "$(SNDIO_PREFIX)/lib/pkgconfig/sndio.pc" || \
		printf 'Requires.private: alsa\n' >> "$(SNDIO_PREFIX)/lib/pkgconfig/sndio.pc"
	@test -x "$(SNDIO_PREFIX)/bin/sndiod" || { printf '[FreeLinX/ports][error] %s: sndiod was not installed under %s/bin\n' "$(NAME)" "$(SNDIO_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged sndio into %s\n' "$(SNDIO_PREFIX)"
