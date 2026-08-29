# FreeLinX/ports - base/mkdir : NetBSD 10.1 `/bin/mkdir`.
#
# Builds the ACTUAL NetBSD mkdir utility source (bin/mkdir/mkdir.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  It is not an invented mkdir or a host coreutils binary.
#
# OS porting notes (see patches/README):
#   - mkdir -m parses symbolic modes with setmode(3)/getmode(3), which musl
#     lacks; NetBSD's own libc lib/libc/gen/setmode.c is linked in from
#     base/compat (same drop-in the netbsd-sh umask builtin uses).
#   - setprogname() comes from base/compat/getprogname.c.
NAME   := mkdir
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/mkdir -> src/rootfs/bin/mkdir.  Set before include.
INSTALL_BIN:= mkdir
INSTALL_RELPATH:= bin/mkdir

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/mkdir/mkdir.c

# mkdir -m needs NetBSD's setmode(3)/getmode(3) (musl lacks them).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/setmode.c

PORT_SRCS = bin/mkdir/mkdir.c