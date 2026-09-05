# FreeLinX/ports - base/hesinfo : NetBSD 10.1 `/usr/bin/hesinfo`.
NAME   := hesinfo
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= hesinfo
INSTALL_RELPATH:= bin/hesinfo

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/hesinfo/hesinfo.c

PORT_SRCS = usr.bin/hesinfo/hesinfo.c
COMPAT_SRCS += $(FLX_COMPAT)/hesiod.c
