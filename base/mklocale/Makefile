# FreeLinX/ports - base/mklocale : NetBSD 10.1 `/usr/bin/mklocale`.
NAME   := mklocale
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mklocale
INSTALL_RELPATH:= bin/mklocale

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/mklocale/ldef.h \
	usr/src/usr.bin/mklocale/mklocaledb.c

PORT_SRCS = usr.bin/mklocale/mklocaledb.c
