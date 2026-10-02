# -*- makefile -*-
# FreeLinX/ports - common make definitions.
#
# Shared defaults for every port Makefile, mirroring config/default.conf.
# Values use `?=` so an explicit make command-line or environment variable
# (FREELINX_*) always wins. No hard-coded, developer-specific paths live here;
# everything is configurable and resolves relative to the repository root.
#
# The shell scripts source config/default.conf directly; make keeps a small,
# documented mirror below. The single readable source table is in README.md.
#
# This framework is intentionally bmake/GNU-make friendly and minimal. It is
# NOT a package manager; it is a BSD-style ports harness.

# ---------------------------------------------------------------------------
# Repository root. $(CURDIR) is this directory when the top-level Makefile
# includes this file; per-port Makefiles include ../../mk/base-port.mk (or
# port.mk) which set FREELINX_PORTS_ROOT, so this is just a safe fallback.
# ---------------------------------------------------------------------------
FREELINX_PORTS_ROOT?=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

# ---------------------------------------------------------------------------
# Host tools (POSIX-flavoured only; GNU-ism-tolerant but not GNU-required).
# ---------------------------------------------------------------------------
CP?=cp
MV?=mv
RM?=rm
MKDIR?=mkdir
SED?=sed
INSTALL?=install
LN?=ln
TAR?=tar

# ---------------------------------------------------------------------------
# Core FreeLinX variables (mirror of config/default.conf). `?=` means an
# environment or command-line override wins.
# ---------------------------------------------------------------------------
FREELINX_ARCH?=x86_64
FREELINX_TRIPLE?=x86_64-linux-musl

FREELINX_TOOLCHAIN_DIR?=$(FREELINX_PORTS_ROOT)/../toolchain
FREELINX_TOOLCHAIN_BIN?=$(FREELINX_TOOLCHAIN_DIR)/bin
FREELINX_CC?=$(FREELINX_TOOLCHAIN_BIN)/clang
FREELINX_CXX?=$(FREELINX_TOOLCHAIN_BIN)/clang++
FREELINX_LD?=$(FREELINX_TOOLCHAIN_BIN)/ld.lld
FREELINX_AR?=$(FREELINX_TOOLCHAIN_BIN)/llvm-ar
FREELINX_STRIP?=$(FREELINX_TOOLCHAIN_BIN)/llvm-strip --strip-debug
FREELINX_SYSROOT?=$(FREELINX_TOOLCHAIN_DIR)/x86_64-linux-musl

FREELINX_LINK_MODE?=static
FREELINX_PREFIX?=$(FREELINX_PORTS_ROOT)/staging/$(FREELINX_TRIPLE)

FREELINX_BUILD_DIR?=$(FREELINX_PORTS_ROOT)/build
FREELINX_DIST_DIR?=$(FREELINX_PORTS_ROOT)/dist
FREELINX_STAGING_DIR?=$(FREELINX_PORTS_ROOT)/staging
FREELINX_WORK_DIR?=$(FREELINX_BUILD_DIR)/work

# The pkg-config search path for a cross build: every library port's staged
# lib/pkgconfig, and nothing else.
#
# PKG_CONFIG_LIBDIR rather than PKG_CONFIG_PATH, and every deps directory
# rather than a per-port list.  Both halves of that matter.
#
#   * PATH *adds* to pkg-config's built-in list, which on this host contains
#     /usr/lib/pkgconfig.  libICE's configure finds the host's libbsd-overlay.pc
#     there, defines LIBBSD_OVERLAY, and the build starts including the host's
#     glibc headers -- which shows up as
#       iceauth.c:83:1: error: static declaration of 'arc4random_buf' follows
#       non-static declaration
#     with a note pointing at /usr/include/bsd/stdlib.h.
#
#   * A per-port list is wrong the moment a dependency's own Requires reach one
#     level further.  libXext's list had x11.pc's directory in it and still
#     failed with "Package requirements (xproto >= 7.0.13 x11 >= 1.6 xextproto
#     >= 7.1.99) were not met", because x11.pc requires kbproto and kbproto's
#     directory was not in the list.
#
# Where two deps stage the same module name the first sorted directory wins,
# and that is the one wanted: xorgproto's xproto.pc, Version 2024.1, is what
# satisfies the xproto >= 7.0.33 that no released xproto does.
# pkg-config for a static-only build: --static so Requires.private and
# Libs.private are followed.  Without it a module reports the library and not
# what the library needs, and a static link stops at
#
#   ld.lld: error: undefined symbol: XauGetBestAuthByAddr
#
# with nothing in the log about pkg-config.  Upstream does not notice, because
# with shared libraries the dependency is recorded in the .so itself.
#
# The flag is delivered by scripts/flx-toolshim/pkg-config, which is first on
# PATH, rather than by putting it in the PKG_CONFIG variable.  pkg.m4's
# PKG_PROG_PKG_CONFIG runs AC_PATH_TOOL, and AC_PATH_TOOL reduces the variable to
# "the first word of pkg-config, so it can be a program name with args" --
#
#     PKG_CONFIG=$ac_pt_PKG_CONFIG
#
# -- so a PKG_CONFIG="/usr/bin/pkg-config --static" on the configure line loses
# the flag before the first PKG_CHECK_MODULES runs, and a build log can record
#
#     ac_cv_env_PKG_CONFIG_value='/usr/bin/pkg-config --static'
#
# while the resolved flags show no sign of it.  Presetting ac_cv_path_PKG_CONFIG
# does not help either: the branch that reaches ac_pt_PKG_CONFIG is chosen by
# whether that variable is empty, not by what it contains.
#
# A shim on PATH cannot be rewritten that way, because AC_PATH_TOOL finds the
# program by searching PATH and all it keeps is the path it found.
FLX_PKG_CONFIG ?= pkg-config
FLX_AC_PATH_PKG_CONFIG ?= $(FREELINX_PORTS_ROOT)/scripts/flx-toolshim/pkg-config
FLX_AC_PATH_AC_PT_PKG_CONFIG ?= $(FREELINX_PORTS_ROOT)/scripts/flx-toolshim/pkg-config
export PATH := $(FREELINX_PORTS_ROOT)/scripts/flx-toolshim:$(PATH)

empty :=
space := $(empty) $(empty)
comma := ,
# xorgproto first, deliberately, and then everything else sorted.  xorgproto
# absorbed the individual *-proto packages and ships a .pc for each of them, at
# their current versions: its inputproto.pc says 2.4.0 where the x11/inputproto
# port's says 2.3.2.  Sorted by name, inputproto comes first, and xorg-server's
# configure then stops with
#
#   Package dependency requirement 'inputproto >= 2.3.99.1' could not be
#   satisfied.  Package 'inputproto' has version '2.3.2'
#
# which names a version nobody can satisfy rather than the directory that was
# in the way.  The include order in mk/xorg-lib.mk puts xorgproto first for the
# same reason: its headers are the newer ones.
FREELINX_ALL_PKGCONFIG_DIRS := $(sort $(wildcard $(FREELINX_BUILD_DIR)/deps/*/lib/pkgconfig))
FREELINX_DEPS_PKGCONFIG_DIRS := $(FREELINX_BUILD_DIR)/deps/xorgproto/lib/pkgconfig \
	$(filter-out $(FREELINX_BUILD_DIR)/deps/xorgproto/lib/pkgconfig,$(FREELINX_ALL_PKGCONFIG_DIRS))
FREELINX_PKGCONFIG_LIBDIR := $(subst $(space),:,$(strip $(FREELINX_DEPS_PKGCONFIG_DIRS)))

# Exported for every port, not just the ones that go through mk/xorg-lib.mk.
#
# A port's own build system can call pkg-config from inside a recipe, where
# neither PKG_CONFIG nor the shim on PATH has any say.  x11/st's config.mk has
#
#   CFLAGS += `pkg-config --cflags fontconfig`
#   LDLIBS += `pkg-config --libs fontconfig`
#
# and with no PKG_CONFIG_LIBDIR in the environment those two run against the
# build host's /usr/lib/pkgconfig.  Its fontconfig.pc is glibc fontconfig's, and
# st's link line came out as
#
#   clang -o st st.o x.o ... `pkg-config --libs fontconfig` `pkg-config --libs freetype2`
#   ld.lld: error: unable to find library -lbz2
#   ld.lld: error: unable to find library -lpng16
#   ld.lld: error: unable to find library -lz
#   ld.lld: error: unable to find library -lbrotlidec
#   ld.lld: error: unable to find library -lbrotlicommon
#   ld.lld: error: unable to find library -lexpat
#
# Six host libraries, named by a module that describes the host's build.  On a
# host that had the static archives this would have linked glibc's zlib, bzip2,
# libpng, brotli and expat into a musl binary, and the host's fontconfig.pc
# carries -I/usr/include/fontconfig, so the compile before it was against
# host headers too.
#
# Recursive (=, not :=) so the directory list is globbed when a recipe's
# environment is built rather than while this file is parsed: a port's
# dependencies are built by earlier make invocations, and their lib/pkgconfig
# directories do not exist yet at the moment the port's Makefile is first read.
export PKG_CONFIG_LIBDIR = $(FREELINX_PKGCONFIG_LIBDIR)
# PKG_CONFIG_PATH is *added* to PKG_CONFIG_LIBDIR rather than replaced by it, so
# an inherited value would put the host's directories back.
export PKG_CONFIG_PATH =

# Rootfs-overlay staging + src rootfs template (configurable, not hard-coded).
FREELINX_STAGING_ROOT?=$(FREELINX_PORTS_ROOT)/staging
FREELINX_SRC_DIR?=$(FREELINX_PORTS_ROOT)/../src
FREELINX_ROOTFS_DIR?=$(FREELINX_SRC_DIR)/rootfs
FREELINX_ROOTFS_BIN?=$(FREELINX_ROOTFS_DIR)/bin

FREELINX_JOBS?=1
