# FreeLinX/ports - base/users : NetBSD 10.1 `/usr/bin/users`.
NAME   := users
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= users
INSTALL_RELPATH:= bin/users

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/users/users.c

PORT_SRCS = usr.bin/users/users.c
COMPAT_SRCS += $(FLX_COMPAT)/utmpentry.c $(FLX_COMPAT)/strlcpy.c
