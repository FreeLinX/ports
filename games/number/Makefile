# FreeLinX/ports - games/number : NetBSD 10.1 `/games/number`.
#
# Builds the ACTUAL NetBSD source (games/number/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := number
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= number
INSTALL_RELPATH:= usr/games/number

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/number

NETBSD_MEMBERS = usr/src/games/number/number.c

PORT_SRCS = games/number/number.c
