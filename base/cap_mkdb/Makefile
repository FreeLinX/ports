# FreeLinX/ports - base/cap_mkdb : NetBSD 10.1 `/usr/bin/cap_mkdb`.
NAME   := cap_mkdb
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= cap_mkdb
INSTALL_RELPATH:= bin/cap_mkdb

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/cap_mkdb/cap_mkdb.c

PORT_SRCS = usr.bin/cap_mkdb/cap_mkdb.c
