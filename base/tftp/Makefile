# FreeLinX/ports - base/tftp : NetBSD 10.1 `/usr/bin/tftp`.
NAME   := tftp
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tftp
INSTALL_RELPATH:= bin/tftp

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tftp/extern.h \
	usr/src/usr.bin/tftp/main.c \
	usr/src/usr.bin/tftp/tftp.c \
	usr/src/usr.bin/tftp/tftpsubs.c \
	usr/src/usr.bin/tftp/tftpsubs.h

PORT_SRCS = usr.bin/tftp/main.c \
	usr.bin/tftp/tftp.c \
	usr.bin/tftp/tftpsubs.c
