# FreeLinX/ports - base/jot : NetBSD 10.1 `/usr/bin/jot`.
#
# Builds the ACTUAL NetBSD jot source (usr.bin/jot/jot.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# Not an invented jot or a host util-linux binary.
#
# libc notes: jot.c uses math(3), random(3), time(3) and err(3); none
# BSD-only.  getprogname(3) from base/compat.
NAME   := jot
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= jot
INSTALL_RELPATH:= bin/jot

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/jot/jot.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/jot/jot.c