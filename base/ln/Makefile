# FreeLinX/ports - base/ln : NetBSD 10.1 `/bin/ln`.
#
# Builds the ACTUAL NetBSD ln utility source (bin/ln/ln.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() and strlcpy() come from base/compat.
#   - symlink()/link()/unlink() are plain POSIX, provided by musl.
NAME   := ln
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/ln -> src/rootfs/bin/ln.
INSTALL_BIN:= ln
INSTALL_RELPATH:= bin/ln

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/ln/ln.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c

PORT_SRCS = bin/ln/ln.c
