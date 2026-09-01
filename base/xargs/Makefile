# FreeLinX/ports - base/xargs : NetBSD 10.1 `/usr/bin/xargs`.
#
# Builds the ACTUAL NetBSD xargs utility source (usr.bin/xargs/{xargs.c,
# strnsubst.c,pathnames.h} from the 10.1 src set) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), static.  Not an invented xargs or a host
# findutils binary.
NAME   := xargs
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= xargs
INSTALL_RELPATH:= bin/xargs

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/xargs/xargs.c \
	usr/src/usr.bin/xargs/strnsubst.c \
	usr/src/usr.bin/xargs/pathnames.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/signalname.c

PORT_SRCS = usr.bin/xargs/xargs.c usr.bin/xargs/strnsubst.c