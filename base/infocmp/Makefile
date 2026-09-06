# FreeLinX/ports - base/infocmp : NetBSD 10.1 `/usr/bin/infocmp`.
NAME   := infocmp
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= infocmp
INSTALL_RELPATH:= bin/infocmp

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/infocmp/infocmp.c

PORT_SRCS = usr.bin/infocmp/infocmp.c

FLX_CPPFLAGS+= $(FLX_TERMINFO_CPPFLAGS)
COMPAT_SRCS += $(FLX_LTERMINFO_SRCS)
