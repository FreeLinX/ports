# FreeLinX/ports - base/extattr : NetBSD 10.1 `/usr/bin/extattr`.
NAME   := extattr
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= extattr
INSTALL_RELPATH:= bin/extattr

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/extattr/getextattr.c

PORT_SRCS = usr.bin/extattr/getextattr.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
COMPAT_SRCS += $(FLX_COMPAT)/vis.c
