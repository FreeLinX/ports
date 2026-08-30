# FreeLinX/ports - base/sed : NetBSD 10.1 `/usr/bin/sed`.
NAME   := sed
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= sed
INSTALL_RELPATH:= bin/sed

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/sed/compile.c usr/src/usr.bin/sed/defs.h \
	usr/src/usr.bin/sed/extern.h usr/src/usr.bin/sed/main.c \
	usr/src/usr.bin/sed/misc.c usr/src/usr.bin/sed/process.c

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/sed/compile.c usr.bin/sed/main.c \
	usr.bin/sed/misc.c usr.bin/sed/process.c
