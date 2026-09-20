# FreeLinX/ports - base/mknod : NetBSD 10.1 `/sbin/mknod`.
#
# Builds the ACTUAL NetBSD source (sbin/mknod/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := mknod
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mknod
INSTALL_RELPATH:= bin/mknod

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/mknod

NETBSD_MEMBERS = usr/src/sbin/mknod/mknod.c \
	usr/src/sbin/mknod/pack_dev.c \
	usr/src/sbin/mknod/pack_dev.h

PORT_SRCS = sbin/mknod/mknod.c \
	sbin/mknod/pack_dev.c
FLX_CPPFLAGS += -include sys/sysmacros.h
COMPAT_SRCS += $(FLX_COMPAT)/setmode.c $(FLX_COMPAT)/sigstub.c
