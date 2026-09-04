# FreeLinX/ports - base/tip : NetBSD 10.1 `/usr/bin/tip`.
NAME   := tip
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tip
INSTALL_RELPATH:= bin/tip

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tip/acu.c \
	usr/src/usr.bin/tip/acutab.c \
	usr/src/usr.bin/tip/cmds.c \
	usr/src/usr.bin/tip/cmdtab.c \
	usr/src/usr.bin/tip/cu.c \
	usr/src/usr.bin/tip/hunt.c \
	usr/src/usr.bin/tip/partab.c \
	usr/src/usr.bin/tip/pathnames.h \
	usr/src/usr.bin/tip/remote.c \
	usr/src/usr.bin/tip/tip.c \
	usr/src/usr.bin/tip/tip.h \
	usr/src/usr.bin/tip/tipout.c \
	usr/src/usr.bin/tip/value.c \
	usr/src/usr.bin/tip/vars.c

PORT_SRCS = usr.bin/tip/acu.c \
	usr.bin/tip/acutab.c \
	usr.bin/tip/cmds.c \
	usr.bin/tip/cmdtab.c \
	usr.bin/tip/cu.c \
	usr.bin/tip/hunt.c \
	usr.bin/tip/partab.c \
	usr.bin/tip/remote.c \
	usr.bin/tip/tip.c \
	usr.bin/tip/tipout.c \
	usr.bin/tip/value.c \
	usr.bin/tip/vars.c
