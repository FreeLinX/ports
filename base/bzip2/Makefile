# FreeLinX/ports - base/bzip2 : NetBSD 10.1 `/usr/bin/bzip2`.
#
# Builds the ACTUAL bzip2 sources as shipped in the NetBSD 10.1 src set
# (external/bsd/bzip2/dist/{bzip2,bzlib,compress,decompress,blocksort,
# huffman,randtable,crctable}.c + bzlib.h/bzlib_private.h) with the FreeLinX
# toolchain (clang + LLD + musl sysroot), static.
#
# NetBSD ships bzip2 under external/bsd/bzip2/dist (its usr.bin/bzip2 dir is
# only a bsd.prog.mk wrapper), so the same libbz2 sources the NetBSD kernel
# and libarchive build against are the ones compiled here.  bzip2.c is the
# standalone driver and the remaining .c files are the libbz2 codec, linked
# together as one static binary exactly like upstream's libbz2.a + bzip2.
#
# OS porting notes:
#   - bzip2 is self-contained portable C (no NetBSD libc extensions), so
#     COMPAT_SRCS is empty.
NAME   := bzip2
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/usr/bin/bzip2 -> src/rootfs/usr/bin/bzip2.
INSTALL_BIN:= bzip2
INSTALL_RELPATH:= usr/bin/bzip2

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/external/bsd/bzip2/dist/bzip2.c \
	usr/src/external/bsd/bzip2/dist/bzlib.c \
	usr/src/external/bsd/bzip2/dist/compress.c \
	usr/src/external/bsd/bzip2/dist/decompress.c \
	usr/src/external/bsd/bzip2/dist/blocksort.c \
	usr/src/external/bsd/bzip2/dist/huffman.c \
	usr/src/external/bsd/bzip2/dist/randtable.c \
	usr/src/external/bsd/bzip2/dist/crctable.c \
	usr/src/external/bsd/bzip2/dist/bzlib.h \
	usr/src/external/bsd/bzip2/dist/bzlib_private.h

COMPAT_SRCS =

PORT_SRCS = external/bsd/bzip2/dist/bzip2.c \
	external/bsd/bzip2/dist/bzlib.c \
	external/bsd/bzip2/dist/compress.c \
	external/bsd/bzip2/dist/decompress.c \
	external/bsd/bzip2/dist/blocksort.c \
	external/bsd/bzip2/dist/huffman.c \
	external/bsd/bzip2/dist/randtable.c \
	external/bsd/bzip2/dist/crctable.c