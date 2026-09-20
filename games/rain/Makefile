# FreeLinX/ports - games/rain : NetBSD 10.1 `/games/rain`.
#
# Builds the ACTUAL NetBSD source (games/rain/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := rain
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rain
INSTALL_RELPATH:= usr/games/rain

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/rain

NETBSD_MEMBERS = usr/src/games/rain/rain.c

PORT_SRCS = games/rain/rain.c
FLX_CPPFLAGS += $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD    += $(FLX_NCURSES)/lib/libncursesw.a
