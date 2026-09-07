# FreeLinX/ports - games/random : NetBSD 10.1 random number game.
#
# Random number guessing game.  BSD license.
# Single-file port; no external library dependencies.

NAME   := random
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= random
INSTALL_RELPATH:= usr/games/random

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/random

NETBSD_MEMBERS = usr/src/games/random/random.c

PORT_SRCS = games/random/random.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/arc4random.c
