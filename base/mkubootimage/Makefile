# FreeLinX/ports - base/mkubootimage : NetBSD 10.1 `/usr/bin/mkubootimage`.
NAME   := mkubootimage
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mkubootimage
INSTALL_RELPATH:= bin/mkubootimage

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/mkubootimage/arm64.h \
	usr/src/usr.bin/mkubootimage/crc32.c \
	usr/src/usr.bin/mkubootimage/crc32.h \
	usr/src/usr.bin/mkubootimage/mkubootimage.c \
	usr/src/usr.bin/mkubootimage/uboot.h

PORT_SRCS = usr.bin/mkubootimage/crc32.c \
	usr.bin/mkubootimage/mkubootimage.c
