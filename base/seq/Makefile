# FreeLinX/ports - base/seq : NetBSD 10.1 `/usr/bin/seq`.
#
# Builds the ACTUAL NetBSD seq source (usr.bin/seq/seq.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# Not an invented seq or a host coreutils binary.
#
# libc notes: seq.c uses strtod(3), locale(3) and err(3); none BSD-only.
# getprogname(3) from base/compat.
NAME   := seq
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= seq
INSTALL_RELPATH:= bin/seq

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/seq/seq.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/seq/seq.c