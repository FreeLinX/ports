# FreeLinX/ports - base/elf2aout : NetBSD 10.1 `/usr/bin/elf2aout`.
NAME   := elf2aout
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= elf2aout
INSTALL_RELPATH:= bin/elf2aout

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/elf2aout/elf2aout.c

PORT_SRCS = usr.bin/elf2aout/elf2aout.c
