# FreeLinX/ports - games/morse : NetBSD 10.1 morse code translator.
#
# Translates text to/from Morse code.  BSD license.
# Single-file port; no external library dependencies.

NAME   := morse
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= morse
INSTALL_RELPATH:= usr/games/morse

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/morse

NETBSD_MEMBERS = usr/src/games/morse/morse.c

PORT_SRCS = games/morse/morse.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c
