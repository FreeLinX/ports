# FreeLinX/ports - base/expand : NetBSD 10.1 `/usr/bin/expand`.
#
# Builds the ACTUAL NetBSD expand source (usr.bin/expand/expand.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented expand or a host coreutils binary.
#
# libc notes: expand.c uses err(3)/errx(3) (musl), strtol(3); none BSD-only.
NAME   := expand
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= expand
INSTALL_RELPATH:= bin/expand

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/expand/expand.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/expand/expand.c