# FreeLinX/ports - base/csplit : NetBSD 10.1 `/usr/bin/csplit`.
NAME   := csplit
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= csplit
INSTALL_RELPATH:= bin/csplit

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/csplit/csplit.c

PORT_SRCS = usr.bin/csplit/csplit.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
