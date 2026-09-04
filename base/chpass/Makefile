# FreeLinX/ports - base/chpass : NetBSD 10.1 `/usr/bin/chpass`.
NAME   := chpass
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= chpass
INSTALL_RELPATH:= bin/chpass

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/chpass/chpass.c \
	usr/src/usr.bin/chpass/chpass.h \
	usr/src/usr.bin/chpass/edit.c \
	usr/src/usr.bin/chpass/field.c \
	usr/src/usr.bin/chpass/pathnames.h \
	usr/src/usr.bin/chpass/pw_yp.c \
	usr/src/usr.bin/chpass/table.c \
	usr/src/usr.bin/chpass/util.c

PORT_SRCS = usr.bin/chpass/chpass.c \
	usr.bin/chpass/edit.c \
	usr.bin/chpass/field.c \
	usr.bin/chpass/pw_yp.c \
	usr.bin/chpass/table.c \
	usr.bin/chpass/util.c
