# FreeLinX/ports - base/id : NetBSD 10.1 `/usr/bin/id`.
#
# Builds the ACTUAL NetBSD id utility source (usr.bin/id/id.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes: none beyond the default compat.  id.c uses
# getprogname(3) (base/compat; also covers the "groups"/"whoami" alias
# dispatch); getgrouplist(3) and the rest are standard musl.
NAME   := id
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/id -> src/rootfs/bin/id.
INSTALL_BIN:= id
INSTALL_RELPATH:= bin/id

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/id/id.c

PORT_SRCS = usr.bin/id/id.c