# FreeLinX/ports - base/cat : NetBSD 10.1 `/bin/cat`.
#
# Builds the ACTUAL NetBSD cat utility source (bin/cat/cat.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# It is not an invented cat or a host coreutils binary.
#
# OS porting notes (see patches/README):
#   - none required: cat.c's `#if HAVE_NBTOOL_CONFIG_H` include of
#     nbtool_config.h is off by default, and the rest fits musl once the
#     FreeLinX musl-compat getprogname(3) shim supplies setprogname().
NAME   := cat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/cat -> src/rootfs/bin/cat.  Set before include.
INSTALL_BIN:= cat
INSTALL_RELPATH:= bin/cat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/cat/cat.c
PORT_SRCS = bin/cat/cat.c