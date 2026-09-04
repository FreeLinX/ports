# FreeLinX/ports - base/rsh : NetBSD 10.1 `/usr/bin/rsh`.
NAME   := rsh
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rsh
INSTALL_RELPATH:= bin/rsh

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/rsh/getport.c \
	usr/src/usr.bin/rsh/getport.h \
	usr/src/usr.bin/rsh/pathnames.h \
	usr/src/usr.bin/rsh/rsh.c

PORT_SRCS = usr.bin/rsh/getport.c \
	usr.bin/rsh/rsh.c
