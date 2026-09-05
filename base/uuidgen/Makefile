# FreeLinX/ports - base/uuidgen : NetBSD 10.1 `/usr/bin/uuidgen`.
NAME   := uuidgen
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= uuidgen
INSTALL_RELPATH:= bin/uuidgen

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/uuidgen/uuidgen.c

PORT_SRCS = usr.bin/uuidgen/uuidgen.c
COMPAT_SRCS += $(FLX_COMPAT)/uuid.c
