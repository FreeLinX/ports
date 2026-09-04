# FreeLinX/ports - base/tset : NetBSD 10.1 `/usr/bin/tset`.
NAME   := tset
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tset
INSTALL_RELPATH:= bin/tset

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tset/extern.h \
	usr/src/usr.bin/tset/map.c \
	usr/src/usr.bin/tset/misc.c \
	usr/src/usr.bin/tset/set.c \
	usr/src/usr.bin/tset/term.c \
	usr/src/usr.bin/tset/tset.c

PORT_SRCS = usr.bin/tset/map.c \
	usr.bin/tset/misc.c \
	usr.bin/tset/set.c \
	usr.bin/tset/term.c \
	usr.bin/tset/tset.c
