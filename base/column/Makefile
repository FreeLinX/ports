# FreeLinX/ports - base/column : NetBSD 10.1 `/usr/bin/column`.
#
# Builds the ACTUAL NetBSD column utility source (usr.bin/column/column.c
# from the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl
# sysroot), static.  Not an invented column or a host coreutils binary.
#
# OS porting notes (see patches/README):
#   - `<util.h>` is not in musl; the FreeLinX compat stand-in (base/compat)
#     supplies it (column pulls getbsize(3)/putenv usage from it).
NAME   := column
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= column
INSTALL_RELPATH:= bin/column

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/column/column.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/estr.c

PORT_SRCS = usr.bin/column/column.c