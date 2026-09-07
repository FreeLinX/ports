# FreeLinX/ports - base/shuffle : NetBSD 10.1 `/usr/bin/shuffle`.
NAME   := shuffle
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= shuffle
INSTALL_RELPATH:= bin/shuffle

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/shuffle/shuffle.c

PORT_SRCS = usr.bin/shuffle/shuffle.c
COMPAT_SRCS += $(FLX_COMPAT)/arc4random.c
