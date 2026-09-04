# FreeLinX/ports - base/col : NetBSD 10.1 `/usr/bin/col`.
NAME   := col
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= col
INSTALL_RELPATH:= bin/col

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/col/col.c

PORT_SRCS = usr.bin/col/col.c
COMPAT_SRCS += $(FLX_COMPAT)/errc.c $(FLX_COMPAT)/strtoi.c
