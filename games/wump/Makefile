# FreeLinX/ports - games/wump : NetBSD 10.1 `/games/wump`.
#
# Builds the ACTUAL NetBSD source (games/wump/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := wump
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= wump
INSTALL_RELPATH:= usr/games/wump

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/wump

NETBSD_MEMBERS = usr/src/games/wump/pathnames.h \
	usr/src/games/wump/wump.c

PORT_SRCS = games/wump/wump.c
FLX_CPPFLAGS += -include fcntl.h
