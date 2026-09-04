# FreeLinX/ports - base/ldd : NetBSD 10.1 `/usr/bin/ldd`.
NAME   := ldd
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ldd
INSTALL_RELPATH:= bin/ldd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ldd/dummy.c \
	usr/src/usr.bin/ldd/ldd.c \
	usr/src/usr.bin/ldd/ldd.h \
	usr/src/usr.bin/ldd/ldd_elfxx.c

PORT_SRCS = usr.bin/ldd/dummy.c \
	usr.bin/ldd/ldd.c \
	usr.bin/ldd/ldd_elfxx.c
