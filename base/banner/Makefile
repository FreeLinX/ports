# FreeLinX/ports - base/banner : NetBSD 10.1 `/usr/bin/banner`.
NAME   := banner
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= banner
INSTALL_RELPATH:= bin/banner

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/banner/banner.c \
	usr/src/usr.bin/banner/banner.h \
	usr/src/usr.bin/banner/chset.c

PORT_SRCS = usr.bin/banner/banner.c \
	usr.bin/banner/chset.c
