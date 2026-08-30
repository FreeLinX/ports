# FreeLinX/ports - base/gzip : NetBSD 10.1 `/usr/bin/gzip`.
#
# Builds the ACTUAL NetBSD gzip source (usr.bin/gzip/gzip.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# It is not a host-gzip clone: gzip.c is the NetBSD one and its compressed
# streams are produced by the bundled libz.
#
# libc pieces gzip pulls in, extracted from the same src set: fts(3)
# (lib/libc/gen/fts.c + include/fts.h, built the nbtool way).
#
# OS porting notes (see patches/):
#   - bzip2/xz/lzip support is compiled out the upstream way (`-DNO_*`).
#     `.Z` (compress, zuncompress.c) and `.pack` (unpack.c) stay on; those
#     decompressors are #included by gzip.c, so they are not separate TUs.
#   - copymodes() is rewritten to st_atim/st_mtim + futimens(2) (musl has
#     no TIMESPEC_TO_TIMEVAL / st_atimespec); fchflags falls away under the
#     HAVE_STRUCT_STAT_ST_FLAGS guard.  DEFFILEMODE and SIGINFO (->SIGUSR1)
#     fallbacks are supplied; musl lacks both.
#   - zlib: static, prebuilt in build/deps/zlib (libz.a before -lc).
#   - strmode/user_from_uid/group_from_gid/setmode/getmode/getprogname come
#     from base/compat (declared by the FreeLinX compat <sys/cdefs.h>).
NAME   := gzip
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), zlib (build/deps)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/gzip.  Set before include.
INSTALL_BIN:= gzip
INSTALL_RELPATH:= bin/gzip

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

NETBSD_MEMBERS = usr/src/usr.bin/gzip/gzip.c \
	usr/src/usr.bin/gzip/zuncompress.c usr/src/usr.bin/gzip/unpack.c \
	usr/src/usr.bin/gzip/unlz.c \
	usr/src/lib/libc/gen/fts.c \
	usr/src/include/fts.h

# Upstream build switches: bzip2 and xz rely on bz2/lzma libs we don't
# carry, and lzip likewise; the .Z and .pack decompressors are kept on.
# _GNU_SOURCE exposes fopencookie(3) in musl's <stdio.h> (musl has no
# BSD funopen(3), which the .Z decompressor used).
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1 -DNO_BZIP2_SUPPORT \
	-DNO_XZ_SUPPORT -DNO_LZ_SUPPORT -D_GNU_SOURCE
FLX_CPPFLAGS += -I$(ZLIB_PREFIX)/include

# fts(3) needs reallocarr(3); the rest is getprogname.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c

PORT_SRCS = usr.bin/gzip/gzip.c lib/libc/gen/fts.c

# Link libz.a before -lc (recursive default FLX_LD from base-port.mk is
# redefined here; the recipe expands $(FLX_LD) late, at link time).
FLX_LD          = $(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) \
			-nostdlib -L$(FREELINX_SYSROOT)/lib \
			$(FLX_CRT) \
			$(OBJ_DIR)/*.o \
			$(ZLIB_PREFIX)/lib/libz.a \
			-lc $(FLX_COMPILER_RT) \
			$(FLX_CRT_END)