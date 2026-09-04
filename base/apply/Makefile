# FreeLinX/ports - base/apply : NetBSD 10.1 `/usr/bin/apply`.
NAME   := apply
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= apply
INSTALL_RELPATH:= bin/apply

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/apply/apply.c

PORT_SRCS = usr.bin/apply/apply.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
