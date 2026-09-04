# FreeLinX/ports - base/msgc : NetBSD 10.1 `/usr/bin/msgc`.
NAME   := msgc
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= msgc
INSTALL_RELPATH:= bin/msgc

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/msgc/defs.h \
	usr/src/usr.bin/msgc/msgdb.c \
	usr/src/usr.bin/msgc/msgdb.h \
	usr/src/usr.bin/msgc/msgmain.c \
	usr/src/usr.bin/msgc/pathnames.h

PORT_SRCS = usr.bin/msgc/msgdb.c \
	usr.bin/msgc/msgmain.c
