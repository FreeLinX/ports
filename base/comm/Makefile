# FreeLinX/ports - base/comm : NetBSD 10.1 `/usr/bin/comm`.
#
# Builds the ACTUAL NetBSD comm utility source (usr.bin/comm/comm.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes: none beyond the default compat.  comm.c uses only
# standard interfaces (getopt(3), strcasecmp(3), strcoll(3), err(3));
# the default getprogname.o compiles in harmlessly but is otherwise unused.
NAME   := comm
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/comm -> src/rootfs/bin/comm.
INSTALL_BIN:= comm
INSTALL_RELPATH:= bin/comm

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/comm/comm.c

PORT_SRCS = usr.bin/comm/comm.c