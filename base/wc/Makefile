# FreeLinX/ports - base/wc : NetBSD 10.1 `/usr/bin/wc`.
#
# Builds the ACTUAL NetBSD wc utility source (usr.bin/wc/wc.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes: none beyond the default compat.  wc.c uses
# getprogname(3) (base/compat) and MAXBSIZE/u_quad_t/__RCSID from the
# FreeLinX compat <sys/cdefs.h> (musl ships no <sys/cdefs.h>).
NAME   := wc
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/wc -> src/rootfs/bin/wc.
INSTALL_BIN:= wc
INSTALL_RELPATH:= bin/wc

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/wc/wc.c

PORT_SRCS = usr.bin/wc/wc.c