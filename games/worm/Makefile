# FreeLinX/ports - games/worm : NetBSD 10.1 `/games/worm`.
#
# Builds the ACTUAL NetBSD source (games/worm/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := worm
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= worm
INSTALL_RELPATH:= usr/games/worm

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/worm

NETBSD_MEMBERS = usr/src/games/worm/worm.c

PORT_SRCS = games/worm/worm.c
FLX_CPPFLAGS += $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD    += $(FLX_NCURSES)/lib/libncursesw.a
