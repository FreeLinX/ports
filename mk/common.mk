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
empty :=
space := $(empty) $(empty)
FREELINX_DEPS_PKGCONFIG_DIRS := $(sort $(wildcard $(FREELINX_BUILD_DIR)/deps/*/lib/pkgconfig))
FREELINX_PKGCONFIG_LIBDIR := $(subst $(space),:,$(strip $(FREELINX_DEPS_PKGCONFIG_DIRS)))

# Rootfs-overlay staging + src rootfs template (configurable, not hard-coded).
FREELINX_STAGING_ROOT?=$(FREELINX_PORTS_ROOT)/staging
FREELINX_SRC_DIR?=$(FREELINX_PORTS_ROOT)/../src
FREELINX_ROOTFS_DIR?=$(FREELINX_SRC_DIR)/rootfs
FREELINX_ROOTFS_BIN?=$(FREELINX_ROOTFS_DIR)/bin

FREELINX_JOBS?=1
