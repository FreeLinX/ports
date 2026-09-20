# FreeLinX/ports - games/factor : NetBSD 10.1 `/games/factor`.
#
# Builds the ACTUAL NetBSD source (games/factor/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := factor
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= factor
INSTALL_RELPATH:= usr/games/factor

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/factor

NETBSD_MEMBERS = usr/src/games/factor/factor.c

PORT_SRCS = games/factor/factor.c \
	games/primes/pr_tbl.c
NETBSD_MEMBERS += usr/src/games/primes/primes.h
FLX_CPPFLAGS += -I$(SRC_DIR)/games/primes
NETBSD_MEMBERS += usr/src/games/primes/pr_tbl.c
