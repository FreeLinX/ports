# FreeLinX/ports - base/mkfifo : NetBSD 10.1 `/usr/bin/mkfifo`.
NAME   := mkfifo
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mkfifo
INSTALL_RELPATH:= bin/mkfifo

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/mkfifo/mkfifo.c

PORT_SRCS = usr.bin/mkfifo/mkfifo.c
COMPAT_SRCS += $(FLX_COMPAT)/setmode.c
