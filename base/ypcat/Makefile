# FreeLinX/ports - base/ypcat : NetBSD 10.1 `/usr/bin/ypcat`.
NAME   := ypcat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ypcat
INSTALL_RELPATH:= bin/ypcat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ypcat/ypalias_init.c \
	usr/src/usr.bin/ypcat/ypalias_init.h \
	usr/src/usr.bin/ypcat/ypcat.c

PORT_SRCS = usr.bin/ypcat/ypalias_init.c \
	usr.bin/ypcat/ypcat.c
