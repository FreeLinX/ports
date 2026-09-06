# FreeLinX/ports - base/fdformat : NetBSD 10.1 `/usr/bin/fdformat`.
NAME   := fdformat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= fdformat
INSTALL_RELPATH:= bin/fdformat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/fdformat/fdformat.c \
	usr/src/usr.bin/fdformat/pathnames.h

PORT_SRCS = usr.bin/fdformat/fdformat.c
COMPAT_SRCS += $(FLX_GETCAP_SRCS)
