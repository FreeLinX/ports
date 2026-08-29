# FreeLinX/ports - base/umount : FreeLinX-native umount (Linux umount2(2)).
#
# NOTE: this is a FreeLinX-native port, NOT the NetBSD umount source.  NetBSD's
# umount uses BSD unmount(2), statvfs, getmntinfo(3), per-type umount_<fs>
# helpers and NFS/RPC; none map to Linux and are not faked.  FreeLinX ships a
# genuine Linux umount2(2) front-end reading /proc/mounts; see umount.c.
#
# Builds with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# Actually unmounting depends on the FreeLinX kernel implementing umount2(2) -
# not present yet, not faked.
#
# Native-port build: SRC_DIR/BUILD_BIN etc. MUST precede the include (see
# base/dmesg/Makefile for why).

NAME   := umount
VERSION:= freelinx
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= umount
INSTALL_RELPATH:= bin/umount

SRC_DIR = $(CURDIR)
OBJ_DIR = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)
NETBSD_MEMBERS =
PORT_SRCS = umount.c
COMPAT_SRCS =

include ../../mk/base-port.mk

.PHONY: do-prepare
do-prepare:
	@: # FreeLinX-native port: no upstream source to fetch/extract/patch
