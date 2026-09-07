# FreeLinX/ports - games/caesar : NetBSD 10.1 Caesar cipher tool.
#
# Caesar cipher encoder/decoder.  BSD license.
# Single-file port; no external library dependencies.

NAME   := caesar
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= caesar
INSTALL_RELPATH:= usr/games/caesar

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/caesar

NETBSD_MEMBERS = usr/src/games/caesar/caesar.c

PORT_SRCS = games/caesar/caesar.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c
