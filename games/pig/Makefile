# FreeLinX/ports - games/pig : NetBSD 10.1 `/games/pig`.
#
# Builds the ACTUAL NetBSD source (games/pig/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := pig
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pig
INSTALL_RELPATH:= usr/games/pig

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/pig

NETBSD_MEMBERS = usr/src/games/pig/pig.c

PORT_SRCS = games/pig/pig.c
