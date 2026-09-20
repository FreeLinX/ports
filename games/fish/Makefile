# FreeLinX/ports - games/fish : NetBSD 10.1 `/games/fish`.
#
# Builds the ACTUAL NetBSD source (games/fish/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := fish
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= fish
INSTALL_RELPATH:= usr/games/fish

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/fish

NETBSD_MEMBERS = usr/src/games/fish/fish.c \
	usr/src/games/fish/pathnames.h

PORT_SRCS = games/fish/fish.c
