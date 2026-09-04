# FreeLinX/ports - base/resize : NetBSD 10.1 `/usr/bin/resize`.
NAME   := resize
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= resize
INSTALL_RELPATH:= bin/resize

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/resize/resize.c \
	usr/src/usr.bin/resize/resize.h

PORT_SRCS = usr.bin/resize/resize.c
