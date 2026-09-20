# FreeLinX/ports - base/unlink : NetBSD 10.1 `/usr.sbin/unlink`.
#
# Builds the ACTUAL NetBSD source (usr.sbin/unlink/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := unlink
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= unlink
INSTALL_RELPATH:= usr/sbin/unlink

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/unlink

NETBSD_MEMBERS = usr/src/usr.sbin/unlink/unlink.c

PORT_SRCS = usr.sbin/unlink/unlink.c
