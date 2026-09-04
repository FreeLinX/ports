# FreeLinX/ports - base/cdplay : NetBSD 10.1 `/usr/bin/cdplay`.
NAME   := cdplay
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= cdplay
INSTALL_RELPATH:= bin/cdplay

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/cdplay/cdplay.c

PORT_SRCS = usr.bin/cdplay/cdplay.c
