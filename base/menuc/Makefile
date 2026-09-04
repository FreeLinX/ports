# FreeLinX/ports - base/menuc : NetBSD 10.1 `/usr/bin/menuc`.
NAME   := menuc
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= menuc
INSTALL_RELPATH:= bin/menuc

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/menuc/avl.c \
	usr/src/usr.bin/menuc/defs.h \
	usr/src/usr.bin/menuc/main.c \
	usr/src/usr.bin/menuc/mdb.c \
	usr/src/usr.bin/menuc/mdb.h \
	usr/src/usr.bin/menuc/pathnames.h \
	usr/src/usr.bin/menuc/util.c

PORT_SRCS = usr.bin/menuc/avl.c \
	usr.bin/menuc/main.c \
	usr.bin/menuc/mdb.c \
	usr.bin/menuc/util.c
