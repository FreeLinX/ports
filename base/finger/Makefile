# FreeLinX/ports - base/finger : NetBSD 10.1 `/usr/bin/finger`.
NAME   := finger
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= finger
INSTALL_RELPATH:= bin/finger

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/finger/extern.h \
	usr/src/usr.bin/finger/finger.c \
	usr/src/usr.bin/finger/finger.h \
	usr/src/usr.bin/finger/lprint.c \
	usr/src/usr.bin/finger/net.c \
	usr/src/usr.bin/finger/sprint.c \
	usr/src/usr.bin/finger/util.c

PORT_SRCS = usr.bin/finger/finger.c \
	usr.bin/finger/lprint.c \
	usr.bin/finger/net.c \
	usr.bin/finger/sprint.c \
	usr.bin/finger/util.c
COMPAT_SRCS += $(FLX_DB_SRCS) $(FLX_VIS_SRCS) $(FLX_REALLOCARR_SRCS) $(FLX_UTMPENTRY_SRCS) $(FLX_SIGSTUB_SRCS)
FLX_CPPFLAGS += -D__DBINTERFACE_PRIVATE
