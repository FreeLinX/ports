# FreeLinX/ports - base/ipcs : NetBSD 10.1 `/usr/bin/ipcs`.
NAME   := ipcs
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ipcs
INSTALL_RELPATH:= bin/ipcs

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ipcs/ipcs.c

PORT_SRCS = usr.bin/ipcs/ipcs.c
COMPAT_SRCS += $(FLX_COMPAT)/sysctl.c
COMPAT_SRCS += $(FLX_COMPAT)/user_from_uid.c
