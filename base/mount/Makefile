# FreeLinX/ports - base/mount : FreeLinX-native mount (Linux mount(2)).
#
# NOTE: this is a FreeLinX-native port, NOT the NetBSD mount source.  NetBSD's
# mount is built on the BSD VFS model (BSD mount(2), struct statvfs + MNT_*
# flags, getmntinfo(3), disklabel, mount_<fstype> helpers), which has no Linux
# equivalent and is not faked.  FreeLinX ships a genuine Linux mount(2)
# front-end reading /proc/mounts; see mount.c.
#
# Builds with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# Actually mounting depends on the FreeLinX kernel implementing mount(2) /
# tmpfs / devtmpfs - not present yet, not faked.
#
# Native-port build: SRC_DIR/BUILD_BIN etc. MUST precede the include, because
# base-port.mk expands $(BUILD_BIN) eagerly when parsing its build rule.

NAME   := mount
VERSION:= freelinx
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mount
INSTALL_RELPATH:= bin/mount

SRC_DIR = $(CURDIR)
OBJ_DIR = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)
NETBSD_MEMBERS =
PORT_SRCS = mount.c
COMPAT_SRCS =

include ../../mk/base-port.mk

.PHONY: do-prepare
do-prepare:
	@: # FreeLinX-native port: no upstream source to fetch/extract/patch
