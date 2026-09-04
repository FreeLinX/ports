# FreeLinX/ports - base/rump_halt : NetBSD 10.1 `/usr/bin/rump_halt`.
NAME   := rump_halt
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rump_halt
INSTALL_RELPATH:= bin/rump_halt

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/rump_halt/rump.halt.c

PORT_SRCS = usr.bin/rump_halt/rump.halt.c
