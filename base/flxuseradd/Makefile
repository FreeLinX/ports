# FreeLinX/ports - base/flxuseradd : create a user account, non-interactively.
#
# FreeLinX-native port.  There is no useradd in the ports tree and no port for
# one: base/passwd is PORT_NOT_PORTABLE because krb5_passwd.c needs Kerberos,
# and it is an interactive program the installer cannot drive.  This is the
# account handling setup-user needs, written against this system's own layout.
#
# It edits /etc/passwd, /etc/group and /etc/shadow by rewriting one line in
# each, in a fixed order: group first, so the primary gid resolves; then the
# account; then the shadow entry.  Each step is skipped if it already exists,
# so re-running is safe and cannot produce a second entry for one name.
#
# Builds with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# IMPORTANT (native-port build): these variables MUST be set BEFORE the
# include below.  base-port.mk expands $(BUILD_BIN) eagerly when it parses its
# `$(BUILD_BIN): do-prepare` rule, so a post-include override would be ignored.

NAME   := flxuseradd
VERSION:= freelinx
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= flxuseradd
INSTALL_RELPATH:= bin/flxuseradd

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
PORT_SRCS = flxuseradd.c
COMPAT_SRCS =

include ../../mk/base-port.mk

# No NetBSD source to extract or patch; do-prepare is a no-op for native ports.
.PHONY: do-prepare
do-prepare:
	@: # FreeLinX-native port: no upstream source to fetch/extract/patch
