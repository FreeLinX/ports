# FreeLinX/ports - games/snake : NetBSD 10.1 `/games/snake`.
#
# Builds the ACTUAL NetBSD source (games/snake/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := snake
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= snake
INSTALL_RELPATH:= usr/games/snake

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/snake

NETBSD_MEMBERS = 

PORT_SRCS = 
