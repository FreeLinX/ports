# FreeLinX/ports - base/aiomixer : NetBSD 10.1 `/usr/bin/aiomixer`.
NAME   := aiomixer
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= aiomixer
INSTALL_RELPATH:= bin/aiomixer

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/aiomixer/app.h \
	usr/src/usr.bin/aiomixer/draw.c \
	usr/src/usr.bin/aiomixer/draw.h \
	usr/src/usr.bin/aiomixer/main.c \
	usr/src/usr.bin/aiomixer/parse.c \
	usr/src/usr.bin/aiomixer/parse.h

PORT_SRCS = usr.bin/aiomixer/draw.c \
	usr.bin/aiomixer/main.c \
	usr.bin/aiomixer/parse.c

FLX_CPPFLAGS+= $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD   += $(FLX_NCURSES_LDADD)
