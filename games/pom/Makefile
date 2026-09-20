# FreeLinX/ports - games/pom : NetBSD 10.1 `/games/pom`.
#
# Builds the ACTUAL NetBSD source (games/pom/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := pom
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pom
INSTALL_RELPATH:= usr/games/pom

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/pom

NETBSD_MEMBERS = usr/src/games/pom/pom.c

PORT_SRCS = games/pom/pom.c
