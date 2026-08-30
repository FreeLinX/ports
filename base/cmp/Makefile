# FreeLinX/ports - base/cmp : NetBSD 10.1 `/usr/bin/cmp`.
NAME   := cmp
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= cmp
INSTALL_RELPATH:= bin/cmp

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/cmp/cmp.c usr/src/usr.bin/cmp/extern.h \
	usr/src/usr.bin/cmp/misc.c usr/src/usr.bin/cmp/regular.c \
	usr/src/usr.bin/cmp/special.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/cmp/cmp.c usr.bin/cmp/misc.c \
	usr.bin/cmp/regular.c usr.bin/cmp/special.c
