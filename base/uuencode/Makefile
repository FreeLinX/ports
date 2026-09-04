# FreeLinX/ports - base/uuencode : NetBSD 10.1 `/usr/bin/uuencode`.
NAME   := uuencode
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= uuencode
INSTALL_RELPATH:= bin/uuencode

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/uuencode/uuencode.c

PORT_SRCS = usr.bin/uuencode/uuencode.c
COMPAT_SRCS += $(FLX_COMPAT)/b64_ntop.c
