# FreeLinX/ports - base/xstr : NetBSD 10.1 `/usr/bin/xstr`.
NAME   := xstr
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= xstr
INSTALL_RELPATH:= bin/xstr

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/xstr/pathnames.h \
	usr/src/usr.bin/xstr/xstr.c

PORT_SRCS = usr.bin/xstr/xstr.c
