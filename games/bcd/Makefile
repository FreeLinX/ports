# FreeLinX/ports - games/bcd : NetBSD 10.1 `/games/bcd`.
#
# Builds the ACTUAL NetBSD source (games/bcd/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := bcd
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= bcd
INSTALL_RELPATH:= usr/games/bcd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/bcd

NETBSD_MEMBERS = usr/src/games/bcd/bcd.c

PORT_SRCS = games/bcd/bcd.c
