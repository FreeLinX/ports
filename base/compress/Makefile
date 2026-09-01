# FreeLinX/ports - base/compress : NetBSD 10.1 `/usr/bin/compress`.
#
# Builds the ACTUAL NetBSD compress source (usr.bin/compress/{compress.c,
# zopen.c} from the 10.1 src set) with the FreeLinX toolchain (clang + LLD +
# musl sysroot), static.  Not an invented compress or a host gzip binary.
NAME   := compress
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= compress
INSTALL_RELPATH:= bin/compress

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/compress/compress.c usr/src/usr.bin/compress/zopen.c

# zopen.c uses funopen(3) (BSD stdio-over-callbacks, wrapped onto musl
# fopencookie by compat/funopen.c) and errno EFTYPE (compat/errno.h).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/funopen.c

PORT_SRCS = usr.bin/compress/compress.c usr.bin/compress/zopen.c