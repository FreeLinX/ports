# FreeLinX/ports - base/pathchk : NetBSD 10.1 `/usr/bin/pathchk`.
NAME   := pathchk
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pathchk
INSTALL_RELPATH:= bin/pathchk

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/pathchk/pathchk.c

PORT_SRCS = usr.bin/pathchk/pathchk.c
