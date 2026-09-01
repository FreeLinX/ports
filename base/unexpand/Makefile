# FreeLinX/ports - base/unexpand : NetBSD 10.1 `/usr/bin/unexpand`.
#
# Builds the ACTUAL NetBSD unexpand source (usr.bin/unexpand/unexpand.c from
# the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented unexpand or a host coreutils binary.
#
# libc notes: unexpand.c #includes <util.h>; musl ships no util.h, so the
# FreeLinX compat <util.h> stand-in is used (only the header is taken; no
# BSD-only functions are called).
NAME   := unexpand
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= unexpand
INSTALL_RELPATH:= bin/unexpand

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/unexpand/unexpand.c

# unexpand.c uses erealloc(3) from NetBSD <libutil.h>: BSD err(1) wrapper over
# realloc() that aborts on failure; musl has no err-based alloc wrappers, so
# FreeLinX provides base/compat/erealloc.c.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/erealloc.c

PORT_SRCS = usr.bin/unexpand/unexpand.c