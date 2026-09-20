# FreeLinX/ports - base/chroot : NetBSD 10.1 `/usr.sbin/chroot`.
#
# Builds the ACTUAL NetBSD source (usr.sbin/chroot/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := chroot
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= chroot
INSTALL_RELPATH:= usr/sbin/chroot

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/chroot

NETBSD_MEMBERS = usr/src/usr.sbin/chroot/chroot.c

PORT_SRCS = usr.sbin/chroot/chroot.c
