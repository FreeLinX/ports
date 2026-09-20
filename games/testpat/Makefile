# FreeLinX/ports - games/testpat : NetBSD 10.1 `/games/testpat`.
#
# Builds the ACTUAL NetBSD source (games/testpat/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := testpat
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= testpat
INSTALL_RELPATH:= usr/games/testpat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/testpat

NETBSD_MEMBERS = usr/src/games/testpat/testpat.c

PORT_SRCS = games/testpat/testpat.c
FLX_CPPFLAGS += $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD    += $(FLX_NCURSES)/lib/libncursesw.a
