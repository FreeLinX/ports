# FreeLinX/ports - base/radioctl : NetBSD 10.1 `/usr/bin/radioctl`.
NAME   := radioctl
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= radioctl
INSTALL_RELPATH:= bin/radioctl

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/radioctl/radioctl.c

PORT_SRCS = usr.bin/radioctl/radioctl.c
