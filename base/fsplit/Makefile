# FreeLinX/ports - base/fsplit : NetBSD 10.1 `/usr/bin/fsplit`.
NAME   := fsplit
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= fsplit
INSTALL_RELPATH:= bin/fsplit

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/fsplit/fsplit.c

PORT_SRCS = usr.bin/fsplit/fsplit.c
