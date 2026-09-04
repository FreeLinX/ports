# FreeLinX/ports - base/elf2ecoff : NetBSD 10.1 `/usr/bin/elf2ecoff`.
NAME   := elf2ecoff
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= elf2ecoff
INSTALL_RELPATH:= bin/elf2ecoff

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/elf2ecoff/elf2ecoff.c

PORT_SRCS = usr.bin/elf2ecoff/elf2ecoff.c
