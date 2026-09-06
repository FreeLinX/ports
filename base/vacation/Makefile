# FreeLinX/ports - base/vacation : NetBSD 10.1 `/usr/bin/vacation`.
NAME   := vacation
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= vacation
INSTALL_RELPATH:= bin/vacation

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/vacation/vacation.c

PORT_SRCS = usr.bin/vacation/vacation.c
COMPAT_SRCS += $(FLX_DB_SRCS)
FLX_CPPFLAGS += -D__DBINTERFACE_PRIVATE
