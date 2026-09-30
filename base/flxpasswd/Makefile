# FreeLinX/ports - base/flxpasswd : set a password in /etc/shadow.
#
# FreeLinX-native port, NOT the NetBSD passwd source.  Two reasons:
#
#   1. base/passwd is PORT_NOT_PORTABLE because one of its five members,
#      krb5_passwd.c, needs Kerberos, which FreeLinX does not ship.  The
#      whole utility is refused over a feature nothing here wants.
#
#   2. NetBSD's passwd is an interactive login program.  The installer has
#      already collected the password and wants it applied, not prompted for
#      again, and must never be able to ask for a tty it does not have.
#      flxpasswd reads one user:password line and writes the hash, so there
#      is no code path that can prompt and hang a boot.
#
# Builds with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# IMPORTANT (native-port build): these variables MUST be set BEFORE the
# include below.  base-port.mk expands $(BUILD_BIN) eagerly when it parses its
# `$(BUILD_BIN): do-prepare` rule, so a post-include override would be ignored.

NAME   := flxpasswd
VERSION:= freelinx
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= flxpasswd
INSTALL_RELPATH:= bin/flxpasswd

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
PORT_SRCS = flxpasswd.c
COMPAT_SRCS =

include ../../mk/base-port.mk

# No NetBSD source to extract or patch; do-prepare is a no-op for native ports.
.PHONY: do-prepare
do-prepare:
	@: # FreeLinX-native port: no upstream source to fetch/extract/patch
