# FreeLinX/ports - base/printf : NetBSD 10.1 `/usr/bin/printf`.
#
# Builds the ACTUAL NetBSD printf source (usr.bin/printf/printf.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented printf or a host coreutils binary.
#
# libc notes: printf.c uses err(3)/errx(3) (present in musl) and
# getprogname(3) (from base/compat); no other BSD-only APIs.
NAME   := printf
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= printf
INSTALL_RELPATH:= bin/printf

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/printf/printf.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/printf/printf.c