# FreeLinX/ports - devel/harfbuzz : HarfBuzz text shaping 14.6.0 (upstream port).
#
# Real upstream source:
#   https://github.com/harfbuzz/harfbuzz/releases/download/14.6.0/harfbuzz-14.6.0.tar.xz
# Built statically with FreeLinX clang + LLD + musl + libc++.
#
# OpenType text shaping engine.  graphics/libass needs it for complex-script
# subtitle shaping.  MIT licensed.
#
# Meson build -- the tarball ships no configure, so this follows the
# graphics/pixman meson pattern: a generated cross file, host meson+ninja as
# drivers (meson is a build-host program that runs on this machine, exactly
# like the cmake that devel/freetype2 already invokes from the host), and
# `ninja install' straight into the build/deps prefix.
#
# WITHOUT glib/gobject/icu: none of them is in this tree, and harfbuzz's
# unicode callbacks work without them (it falls back to its own callback
# tables).  -Dglib=disabled -Dgobject=disabled -Dicu=disabled, plus cairo,
# chafa, docs, tests, introspection and utilities off: this port delivers the
# shaping library and its subset library, not the test tools.
#
# A library port.  It produces libharfbuzz.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := harfbuzz
VERSION := 14.6.0
CATEGORY:= devel
LICENSE := MIT
DEPENDENCIES:= freetype2
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

HARFBUZZ_PREFIX = $(FREELINX_BUILD_DIR)/deps/harfbuzz

# devel/freetype2 publishes build/deps/freetype2, like every other library
# port in this tree.  freetype2.pc's own Cflags names include/freetype2,
# which is not where the port stages its headers, so the include directory
# is named explicitly here rather than trusted to pkg-config.
PREFIX_freetype2 = $(FREELINX_BUILD_DIR)/deps/freetype2

# harfbuzz is C++; the framework links libc++/libc++abi/libunwind.
# Declared before the include: project-port.mk only adds them when set.
FLX_PROJECT_CXX = yes

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/harfbuzz-$(VERSION)
PROJECT_LIB = $(SRC_TREE)/build/flx/src/libharfbuzz.a
BUILD_BIN   = $(PROJECT_LIB)

# The cross file is mandatory, not decoration: without one meson believes it
# is configuring a native build and tries to *run* the sanity-check binary it
# just compiled, which is a musl static executable that cannot run on the
# build host (see x11/dbus).  It is generated rather than checked in because
# it names absolute toolchain paths.
#
# pkgconfig names this tree's own shim rather than 'false': harfbuzz finds
# freetype2 through pkg-config, and the shim always passes --static with
# PKG_CONFIG_LIBDIR covering every build/deps prefix and nothing else, so no
# host directory can leak back in (see mk/common.mk).
#
# One flag per c_args element: meson hands each element to the compiler as a
# single argument, so two flags in one element arrive with a space inside and
# --sysroot is swallowed (see scripts/check-port-makefiles.py).
#
# cpp_args carries -stdlib=libc++ plus -nostdinc++ and the sysroot's libc++
# include directory: without them clang++ falls back to the host's libstdc++
# and harfbuzz comes out GCC-built (see x11/dbus).
define MESON_CROSS
[binaries]
c = '$(FREELINX_CC)'
cpp = '$(FREELINX_CXX)'
ar = '$(FREELINX_AR)'
strip = 'true'
pkgconfig = '$(FREELINX_PORTS_ROOT)/scripts/flx-toolshim/pkg-config'
[built-in options]
c_args = ['--target=$(FREELINX_TRIPLE)', '--sysroot=$(FREELINX_SYSROOT)', '$(FREELINX_RESOURCE_FLAGS)', '-O2', '-fPIC', '-I$(PREFIX_freetype2)/include']
c_link_args = ['--target=$(FREELINX_TRIPLE)', '--sysroot=$(FREELINX_SYSROOT)', '$(FREELINX_RESOURCE_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt']
cpp_args = ['--target=$(FREELINX_TRIPLE)', '--sysroot=$(FREELINX_SYSROOT)', '$(FREELINX_RESOURCE_FLAGS)', '-O2', '-fPIC', '-stdlib=libc++', '-nostdinc++', '-isystem', '$(FREELINX_SYSROOT)/include/c++/v1', '-I$(PREFIX_freetype2)/include']
cpp_link_args = ['--target=$(FREELINX_TRIPLE)', '--sysroot=$(FREELINX_SYSROOT)', '$(FREELINX_RESOURCE_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt']
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
		--prefix="$(HARFBUZZ_PREFIX)" \
		-Dglib=disabled \
		-Dgobject=disabled \
		-Dicu=disabled \
		-Dcairo=disabled \
		-Dchafa=disabled \
		-Dfreetype=enabled \
		-Dtests=disabled \
		-Dintrospection=disabled \
		-Ddocs=disabled \
		-Dutilities=disabled \
		-Dbenchmark=disabled

BUILD_CMDS = \
	ninja -C build/flx -j"$(FREELINX_JOBS)" && \
	ninja -C build/flx install

# A library stages itself: there is no single binary for install.mk to copy.
# `ninja install' above already installs the archives, the headers and the
# pkg-config files under $(HARFBUZZ_PREFIX); this recipe verifies every piece
# a consumer needs is really there.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@test -f "$(HARFBUZZ_PREFIX)/lib/libharfbuzz.a" || { printf '[FreeLinX/ports][error] %s: libharfbuzz.a was not installed under %s\n' "$(NAME)" "$(HARFBUZZ_PREFIX)" >&2; exit 1; }
	@test -f "$(HARFBUZZ_PREFIX)/include/harfbuzz/hb.h" || { printf '[FreeLinX/ports][error] %s: no headers at %s/include/harfbuzz\n' "$(NAME)" "$(HARFBUZZ_PREFIX)" >&2; exit 1; }
	@test -f "$(HARFBUZZ_PREFIX)/lib/pkgconfig/harfbuzz.pc" || { printf '[FreeLinX/ports][error] %s: harfbuzz.pc was not installed under %s\n' "$(NAME)" "$(HARFBUZZ_PREFIX)" >&2; exit 1; }
	@printf '[FreeLinX/ports] staged harfbuzz into %s\n' "$(HARFBUZZ_PREFIX)"
