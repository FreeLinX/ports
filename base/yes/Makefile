# FreeLinX/ports - base/yes : NetBSD 10.1 `/usr/bin/yes`.
NAME   := yes
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /
INSTALL_BIN:= yes
INSTALL_RELPATH:= bin/yes

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/yes/yes.c
PORT_SRCS = usr.bin/yes/yes.c
