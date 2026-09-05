# FreeLinX/ports - base/tcopy : NetBSD 10.1 `/usr/bin/tcopy`.
NAME   := tcopy
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tcopy
INSTALL_RELPATH:= bin/tcopy

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tcopy/tcopy.c

PORT_SRCS = usr.bin/tcopy/tcopy.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
