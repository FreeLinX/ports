# FreeLinX/ports - base/kdump : NetBSD 10.1 `/usr/bin/kdump`.
NAME   := kdump
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= kdump
INSTALL_RELPATH:= bin/kdump

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/kdump/kdump.c \
	usr/src/usr.bin/kdump/setemul.c \
	usr/src/usr.bin/kdump/setemul.h

PORT_SRCS = usr.bin/kdump/kdump.c \
	usr.bin/kdump/setemul.c
