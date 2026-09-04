# FreeLinX/ports - base/m4 : NetBSD 10.1 `/usr/bin/m4`.
NAME   := m4
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= m4
INSTALL_RELPATH:= bin/m4

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/m4/eval.c \
	usr/src/usr.bin/m4/expr.c \
	usr/src/usr.bin/m4/extern.h \
	usr/src/usr.bin/m4/gnum4.c \
	usr/src/usr.bin/m4/look.c \
	usr/src/usr.bin/m4/main.c \
	usr/src/usr.bin/m4/mdef.h \
	usr/src/usr.bin/m4/misc.c \
	usr/src/usr.bin/m4/pathnames.h \
	usr/src/usr.bin/m4/stdd.h \
	usr/src/usr.bin/m4/trace.c

PORT_SRCS = usr.bin/m4/eval.c \
	usr.bin/m4/expr.c \
	usr.bin/m4/gnum4.c \
	usr.bin/m4/look.c \
	usr.bin/m4/main.c \
	usr.bin/m4/misc.c \
	usr.bin/m4/trace.c
