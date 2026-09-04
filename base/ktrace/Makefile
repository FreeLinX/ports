# FreeLinX/ports - base/ktrace : NetBSD 10.1 `/usr/bin/ktrace`.
NAME   := ktrace
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ktrace
INSTALL_RELPATH:= bin/ktrace

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ktrace/ktrace.c \
	usr/src/usr.bin/ktrace/ktrace.h \
	usr/src/usr.bin/ktrace/subr.c

PORT_SRCS = usr.bin/ktrace/ktrace.c \
	usr.bin/ktrace/subr.c
