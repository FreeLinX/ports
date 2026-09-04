# FreeLinX/ports - base/uudecode : NetBSD 10.1 `/usr/bin/uudecode`.
NAME   := uudecode
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= uudecode
INSTALL_RELPATH:= bin/uudecode

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/uudecode/uudecode.c

PORT_SRCS = usr.bin/uudecode/uudecode.c
COMPAT_SRCS += $(FLX_COMPAT)/b64_ntop.c
