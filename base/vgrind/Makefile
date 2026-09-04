# FreeLinX/ports - base/vgrind : NetBSD 10.1 `/usr/bin/vgrind`.
NAME   := vgrind
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= vgrind
INSTALL_RELPATH:= bin/vgrind

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/vgrind/extern.h \
	usr/src/usr.bin/vgrind/pathnames.h \
	usr/src/usr.bin/vgrind/regexp.c \
	usr/src/usr.bin/vgrind/vfontedpr.c

PORT_SRCS = usr.bin/vgrind/regexp.c \
	usr.bin/vgrind/vfontedpr.c
