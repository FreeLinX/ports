# FreeLinX/ports - base/nbsvtool : NetBSD 10.1 `/usr/bin/nbsvtool`.
NAME   := nbsvtool
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= nbsvtool
INSTALL_RELPATH:= bin/nbsvtool

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/nbsvtool/nbsvtool.c

PORT_SRCS = usr.bin/nbsvtool/nbsvtool.c
