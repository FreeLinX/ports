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
FREELINX_LD?=$(FREELINX_TOOLCHAIN_BIN)/ld.lld
FREELINX_AR?=$(FREELINX_TOOLCHAIN_BIN)/llvm-ar
FREELINX_SYSROOT?=$(FREELINX_TOOLCHAIN_DIR)/x86_64-linux-musl

FREELINX_LINK_MODE?=static
FREELINX_PREFIX?=$(FREELINX_PORTS_ROOT)/staging/$(FREELINX_TRIPLE)

FREELINX_BUILD_DIR?=$(FREELINX_PORTS_ROOT)/build
FREELINX_DIST_DIR?=$(FREELINX_PORTS_ROOT)/dist
FREELINX_STAGING_DIR?=$(FREELINX_PORTS_ROOT)/staging
FREELINX_WORK_DIR?=$(FREELINX_BUILD_DIR)/work

# Rootfs-overlay staging + src rootfs template (configurable, not hard-coded).
FREELINX_STAGING_ROOT?=$(FREELINX_PORTS_ROOT)/staging
FREELINX_SRC_DIR?=$(FREELINX_PORTS_ROOT)/../src
FREELINX_ROOTFS_DIR?=$(FREELINX_SRC_DIR)/rootfs
FREELINX_ROOTFS_BIN?=$(FREELINX_ROOTFS_DIR)/bin

FREELINX_JOBS?=1
