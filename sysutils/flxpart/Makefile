# FreeLinX/ports - sysutils/flxpart : partition a disk for FreeLinX.
#
# Writes a GPT with a protective MBR, so the same disk boots on BIOS and on
# UEFI.  FreeLinX-native: the portable tools for this (sgdisk, parted, fdisk)
# are GPL, and partitioning a disk is the step where reaching for one is most
# tempting and most likely to become a quiet dependency.
#
# The FreeLinX-native flxpart that shipped in the rootfs template was a static
# GCC 16.2.1 binary.  That is a violation of the one rule this project does not
# bend -- nothing in the image may be built by GCC -- and it had no source in
# any repository.  This is the replacement, built with clang and musl like
# everything else.
#
# Layout, all 1 MiB aligned:
#   1  EFI system   FAT32, 256 MiB by default, the UEFI boot target
#   2  BIOS boot    1 MiB, where a BIOS bootloader goes
#   3  FreeLinX     the rest of the disk
#
# It draws a partition table and stops.  mkfs and the system copy are separate
# steps, so a failure after this leaves a partitioned empty disk rather than a
# half-overwritten one.
#
# Builds with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# IMPORTANT (native-port build): these variables MUST be set BEFORE the
# include below.  base-port.mk expands $(BUILD_BIN) eagerly when it parses its
# `$(BUILD_BIN): do-prepare` rule, so a post-include override would be ignored.

NAME   := flxpart
VERSION:= 1.0
CATEGORY:= sysutils
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /sbin

INSTALL_BIN:= flxpart
INSTALL_RELPATH:= sbin/flxpart

# This port builds its binary straight into the tree, so strip it: unstripped
# clang output carries the build path and is ~30% larger, which shows up as a
# spurious diff on every build.
#
# Recursively expanded, not ':='.  mk/common.mk is what defines
# FREELINX_STRIP, and it is pulled in by the include below, so a simple
# assignment here would freeze an empty value and the strip would never run.
STRIP_CMD = $(FREELINX_STRIP)

# FreeLinX-native port: the source lives in this directory, not the NetBSD
# src set.  Point the build at the port's own dir and skip NetBSD extraction.
SRC_DIR = $(CURDIR)
OBJ_DIR = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)
NETBSD_MEMBERS =
PORT_SRCS = flxpart.c
COMPAT_SRCS =

include ../../mk/base-port.mk

# No NetBSD source to extract or patch; do-prepare is a no-op for native ports.
.PHONY: do-prepare
do-prepare:
	@: # FreeLinX-native port: no upstream source to fetch/extract/patch
