# FreeLinX/ports - base/innetgr : NetBSD 10.1 `/usr/bin/innetgr`.
NAME   := innetgr
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= innetgr
INSTALL_RELPATH:= bin/innetgr

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/innetgr/innetgr.c

PORT_SRCS = usr.bin/innetgr/innetgr.c
COMPAT_SRCS += $(FLX_COMPAT)/netgroup.c
