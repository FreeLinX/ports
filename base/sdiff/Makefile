# FreeLinX/ports - base/sdiff : NetBSD 10.1 `/usr/bin/sdiff`.
NAME   := sdiff
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= sdiff
INSTALL_RELPATH:= bin/sdiff

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/sdiff/common.c \
	usr/src/usr.bin/sdiff/common.h \
	usr/src/usr.bin/sdiff/edit.c \
	usr/src/usr.bin/sdiff/extern.h \
	usr/src/usr.bin/sdiff/sdiff.c

PORT_SRCS = usr.bin/sdiff/common.c \
	usr.bin/sdiff/edit.c \
	usr.bin/sdiff/sdiff.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c $(FLX_COMPAT)/fparseln.c
