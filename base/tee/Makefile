# FreeLinX/ports - base/tee : NetBSD 10.1 `/usr/bin/tee`.
#
# Builds the ACTUAL NetBSD tee utility source (usr.bin/tee/tee.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes (patches/patch-tee-deffilemode):
#   - NetBSD's <sys/stat.h> defines DEFFILEMODE (0666) for open(2) mode
#     arguments; musl has no such constant, so tee.c supplies its own.
NAME   := tee
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/tee -> src/rootfs/bin/tee.
INSTALL_BIN:= tee
INSTALL_RELPATH:= bin/tee

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tee/tee.c

PORT_SRCS = usr.bin/tee/tee.c