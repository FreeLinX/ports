# FreeLinX/ports - games/arithmetic : NetBSD 10.1 `/games/arithmetic`.
#
# Builds the ACTUAL NetBSD source (games/arithmetic/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := arithmetic
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= arithmetic
INSTALL_RELPATH:= usr/games/arithmetic

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/arithmetic

NETBSD_MEMBERS = usr/src/games/arithmetic/arithmetic.c

PORT_SRCS = games/arithmetic/arithmetic.c
