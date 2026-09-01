# FreeLinX/ports - base/iconv : NetBSD 10.1 `/usr/bin/iconv`.
#
# Builds the ACTUAL NetBSD iconv utility source (usr.bin/iconv/iconv.c from
# the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented iconv or a host glibc iconv wrapper.
#
# OS porting notes (see patches/README):
#   - `<util.h>` is not in musl; the FreeLinX compat stand-in (base/compat)
#     supplies it (iconv pulls emalloc(3)/estrdup(3) from it).
NAME   := iconv
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= iconv
INSTALL_RELPATH:= bin/iconv

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/iconv/iconv.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/estr.c

PORT_SRCS = usr.bin/iconv/iconv.c