# FreeLinX/ports - base/mkdep : NetBSD 10.1 `/usr/bin/mkdep`.
NAME   := mkdep
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mkdep
INSTALL_RELPATH:= bin/mkdep

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/mkdep/findcc.c \
	usr/src/usr.bin/mkdep/findcc.h \
	usr/src/usr.bin/mkdep/mkdep.c

PORT_SRCS = usr.bin/mkdep/findcc.c \
	usr.bin/mkdep/mkdep.c
