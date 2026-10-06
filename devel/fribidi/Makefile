# FreeLinX/ports - devel/fribidi : FriBidi bidirectional text 1.0.17 (upstream port).
#
# Real upstream source:
#   https://github.com/fribidi/fribidi/releases/download/v1.0.17/fribidi-1.0.17.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Implementation of the Unicode bidirectional algorithm.  graphics/libass
# needs it for right-to-left subtitle shaping.  LGPL-2.1.
#
# Meson build -- the tarball ships no configure, so this follows the
# graphics/pixman meson pattern: a generated cross file, host meson+ninja as
# drivers (meson is a build-host program that runs on this machine, exactly
# like the cmake that devel/freetype2 already invokes from the host), and
# `ninja install' straight into the build/deps prefix.  No dependencies.
#
# -Dbin=false: the fribidi CLI tool is built for the test setup either way
# but not installed, so the deps prefix holds only the library.  -Ddocs=false
# -Dtests=false: no docs or test runners in a static image.
#
# A library port.  It produces libfribidi.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := fribidi
VERSION := 1.0.17
CATEGORY:= devel
LICENSE := LGPL-2.1
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), meson (host), ninja (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
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

FRIBIDI_PREFIX = $(FREELINX_BUILD_DIR)/deps/fribidi

# fribidi is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/fribidi-$(VERSION)
PROJECT_LIB = $(SRC_TREE)/build/flx/lib/libfribidi.a
BUILD_BIN   = $(PROJECT_LIB)

# The cross file is mandatory, not decoration: without one meson believes it
# is configuring a native build and tries to *run* the sanity-check binary it
# just compiled, which is a musl static executable that cannot run on the
# build host (see x11/dbus).  It is generated rather than checked in because
# it names absolute toolchain paths.
#
# pkgconfig names this tree's own shim rather than 'false': the shim always
# passes --static with PKG_CONFIG_LIBDIR covering every build/deps prefix
# and nothing else, so no host directory can leak back in (see mk/common.mk).
#
# One flag per c_args element: meson hands each element to the compiler as a
# single argument, so two flags in one element arrive with a space inside and
# --sysroot is swallowed (see scripts/check-port-makefiles.py).
define MESON_CROSS
[binaries]
c = '$(FREELINX_CC)'
cpp = '$(FREELINX_CXX)'
ar = '$(FREELINX_AR)'
strip = 'true'
pkgconfig = '$(FREELINX_PORTS_ROOT)/scripts/flx-toolshim/pkg-config'
[built-in options]
c_args = ['--target=$(FREELINX_TRIPLE)', '--sysroot=$(FREELINX_SYSROOT)', '$(FREELINX_RESOURCE_FLAGS)', '-O2', '-fPIC']
c_link_args = ['--target=$(FREELINX_TRIPLE)', '--sysroot=$(FREELINX_SYSROOT)', '$(FREELINX_RESOURCE_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt']
[host_machine]
system = 'linux'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'
endef
export MESON_CROSS

PROJECT_CFG_CMDS = \
	printf '%s\n' "$$MESON_CROSS" > flx-cross.ini && \
	rm -rf build/flx && \
	meson setup build/flx \
		--cross-file=flx-cross.ini \
		--default-library=static \
		--buildtype=release \
		--prefix="$(FRIBIDI_PREFIX)" \
		-Ddocs=false \
		-Dbin=false \
		-Dtests=false

BUILD_CMDS = \
	ninja -C build/flx -j"$(FREELINX_JOBS)" && \
	ninja -C build/flx install

# A library stages itself: there is no single binary for install.mk to copy.
# `ninja install' above already installs the archive, the headers and the
# pkg-config files under $(FRIBIDI_PREFIX); this recipe verifies every piece
# a consumer needs is really there.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(FRIBIDI_PREFIX)/lib/libfribidi.a" || { printf '[FreeLinX/ports][error] %s: libfribidi.a was not installed under %s\n' "$(NAME)" "$(FRIBIDI_PREFIX)" >&2; exit 1; }
	@test -f "$(FRIBIDI_PREFIX)/include/fribidi/fribidi.h" || { printf '[FreeLinX/ports][error] %s: no headers at %s/include/fribidi\n' "$(NAME)" "$(FRIBIDI_PREFIX)" >&2; exit 1; }
	@test -f "$(FRIBIDI_PREFIX)/lib/pkgconfig/fribidi.pc" || { printf '[FreeLinX/ports][error] %s: fribidi.pc was not installed under %s\n' "$(NAME)" "$(FRIBIDI_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged fribidi into %s\n' "$(FRIBIDI_PREFIX)"
