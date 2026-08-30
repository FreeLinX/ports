# FreeLinX/ports - base/head : NetBSD 10.1 `/usr/bin/head`.
#
# Builds the ACTUAL NetBSD head utility source (usr.bin/head/head.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes: none beyond the default compat.  head.c uses
# getprogname(3) (base/compat) and getopt(3)/strtoimax(3)/err(3), all
# standard.
NAME   := head
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/head -> src/rootfs/bin/head.
INSTALL_BIN:= head
INSTALL_RELPATH:= bin/head

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/head/head.c

PORT_SRCS = usr.bin/head/head.c