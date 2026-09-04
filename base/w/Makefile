# FreeLinX/ports - base/w : NetBSD 10.1 `/usr/bin/w`.
NAME   := w
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= w
INSTALL_RELPATH:= bin/w

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/w/extern.h \
	usr/src/usr.bin/w/pr_time.c \
	usr/src/usr.bin/w/w.c

PORT_SRCS = usr.bin/w/pr_time.c \
	usr.bin/w/w.c
COMPAT_SRCS += $(FLX_COMPAT)/utmpentry.c $(FLX_COMPAT)/strlcpy.c
