# FreeLinX/ports - games/ppt : NetBSD 10.1 paper, rock, scissors.
#
# Paper, rock, scissors game.  BSD license.
# Single-file port; no external library dependencies.

NAME   := ppt
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ppt
INSTALL_RELPATH:= usr/games/ppt

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/ppt

NETBSD_MEMBERS = usr/src/games/ppt/ppt.c

PORT_SRCS = games/ppt/ppt.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c
