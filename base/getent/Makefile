# FreeLinX/ports - base/getent : NetBSD 10.1 `/usr/bin/getent`.
NAME   := getent
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= getent
INSTALL_RELPATH:= bin/getent

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/getent/getent.c

PORT_SRCS = usr.bin/getent/getent.c
COMPAT_SRCS += $(FLX_COMPAT)/netgroup.c
COMPAT_SRCS += $(FLX_COMPAT)/rpc/rpcent.c
COMPAT_SRCS += $(FLX_COMPAT)/ethernet.c
COMPAT_SRCS += $(FLX_COMPAT)/getcap.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
