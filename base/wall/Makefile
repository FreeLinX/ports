# FreeLinX/ports - base/wall : NetBSD 10.1 `/usr/bin/wall`.
NAME   := wall
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= wall
INSTALL_RELPATH:= bin/wall

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/wall/wall.c usr/src/usr.bin/write/term_chk.h usr/src/usr.bin/write/term_chk.c

PORT_SRCS = usr.bin/wall/wall.c
COMPAT_SRCS += $(FLX_COMPAT)/utmpentry.c $(FLX_COMPAT)/ttymsg.c $(FLX_COMPAT)/strlcpy.c
PORT_SRCS += usr.bin/write/term_chk.c
FLX_CPPFLAGS += -I$(SRC_DIR)/usr.bin/write
