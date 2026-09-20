# FreeLinX/ports - games/worms : NetBSD 10.1 `/games/worms`.
#
# Builds the ACTUAL NetBSD source (games/worms/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := worms
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= worms
INSTALL_RELPATH:= usr/games/worms

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/worms

NETBSD_MEMBERS = usr/src/games/worms/worms.c

PORT_SRCS = games/worms/worms.c
FLX_CPPFLAGS += $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD    += $(FLX_NCURSES)/lib/libncursesw.a
