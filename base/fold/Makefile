# FreeLinX/ports - base/fold : NetBSD 10.1 `/usr/bin/fold`.
#
# Builds the ACTUAL NetBSD fold source (usr.bin/fold/fold.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# Not an invented fold or a host coreutils binary.
#
# libc notes: fold.c uses fgetwc(3)/fputwc(3) locale handling and err(3); all
# present in musl.  getprogname(3) from base/compat.
NAME   := fold
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= fold
INSTALL_RELPATH:= bin/fold

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/fold/fold.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/fold/fold.c