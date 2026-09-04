# FreeLinX/ports - base/rdist : NetBSD 10.1 `/usr/bin/rdist`.
NAME   := rdist
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rdist
INSTALL_RELPATH:= bin/rdist

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/rdist/defs.h \
	usr/src/usr.bin/rdist/docmd.c \
	usr/src/usr.bin/rdist/expand.c \
	usr/src/usr.bin/rdist/lookup.c \
	usr/src/usr.bin/rdist/main.c \
	usr/src/usr.bin/rdist/pathnames.h \
	usr/src/usr.bin/rdist/server.c

PORT_SRCS = usr.bin/rdist/docmd.c \
	usr.bin/rdist/expand.c \
	usr.bin/rdist/lookup.c \
	usr.bin/rdist/main.c \
	usr.bin/rdist/server.c
