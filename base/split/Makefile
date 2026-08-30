# FreeLinX/ports - base/split : NetBSD 10.1 `/usr/bin/split`.
#
# Builds the ACTUAL NetBSD split utility source (usr.bin/split/split.c from
# the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes (patches/patch-split-deffilemode):
#   - NetBSD's <sys/stat.h> defines DEFFILEMODE (0666); musl has no such
#     constant, so split.c supplies its own.  MAXBSIZE comes from the
#     FreeLinX compat <sys/cdefs.h> (musl ships no <sys/cdefs.h>).
NAME   := split
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/split -> src/rootfs/bin/split.
INSTALL_BIN:= split
INSTALL_RELPATH:= bin/split

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/split/split.c

PORT_SRCS = usr.bin/split/split.c