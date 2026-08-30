# FreeLinX/ports - base/rmdir : NetBSD 10.1 `/bin/rmdir`.
#
# Builds the ACTUAL NetBSD rmdir utility source (bin/rmdir/rmdir.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes: none beyond the default compat.  rmdir.c uses
# setprogname(3)/getprogname(3) (supplied by base/compat/getprogname.c); the
# rest of its libc surface (err(3)/warn(3), locale, rmdir(2)) is standard.
NAME   := rmdir
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/rmdir -> src/rootfs/bin/rmdir.
INSTALL_BIN:= rmdir
INSTALL_RELPATH:= bin/rmdir

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/rmdir/rmdir.c

PORT_SRCS = bin/rmdir/rmdir.c