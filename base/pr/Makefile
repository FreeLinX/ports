# FreeLinX/ports - base/pr : NetBSD 10.1 `/usr/bin/pr`.
NAME   := pr
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pr
INSTALL_RELPATH:= bin/pr

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/pr/egetopt.c \
	usr/src/usr.bin/pr/extern.h \
	usr/src/usr.bin/pr/pr.c \
	usr/src/usr.bin/pr/pr.h

PORT_SRCS = usr.bin/pr/egetopt.c \
	usr.bin/pr/pr.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
