# FreeLinX/ports - base/sort : NetBSD 10.1 `/usr/bin/sort`.
#
# Builds the ACTUAL NetBSD sort source (usr.bin/sort/*.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.  It
# is not an invented sort or a host coreutils binary.
#
# OS porting notes:
#   - NetBSD sort links tmp.c's emalloc(3)/erealloc(3) from libutil; the
#     implementations come from lib/libutil/efun.c, extracted from the same
#     src set (NetBSD's own libutil boots this set of e* helpers).
#   - patches/patch-sort-emalloc-decl declares emalloc/erealloc in sort.h
#     (NetBSD gets the prototypes via <util.h>; the FreeLinX compat <util.h>
#     stand-in only declares getbsize, since musl ships no util.h).
#   - patches/patch-efun-unused drops the BSD-only __unused attribute on a
#     function parameter (not defined in musl / the FreeLinX cdefs).
#   - efun.c's ereallocarr needs reallocarr(3) (base/compat); strlcpy(3)/
#     strlcat(3)/vasprintf(3) come from musl libc.
NAME   := sort
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/sort.  Set before include.
INSTALL_BIN:= sort
INSTALL_RELPATH:= bin/sort

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/sort/append.c usr/src/usr.bin/sort/fields.c \
	usr/src/usr.bin/sort/files.c usr/src/usr.bin/sort/fsort.c \
	usr/src/usr.bin/sort/fsort.h usr/src/usr.bin/sort/init.c \
	usr/src/usr.bin/sort/msort.c usr/src/usr.bin/sort/pathnames.h \
	usr/src/usr.bin/sort/radix_sort.c usr/src/usr.bin/sort/sort.c \
	usr/src/usr.bin/sort/sort.h usr/src/usr.bin/sort/tmp.c \
	usr/src/lib/libutil/efun.c

# musl lacks setprogname/getprogname (base/compat) and efun(3)'s ereallocarr
# needs reallocarr(3) (base/compat).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c

PORT_SRCS = usr.bin/sort/append.c usr.bin/sort/fields.c usr.bin/sort/files.c \
	usr.bin/sort/fsort.c usr.bin/sort/init.c usr.bin/sort/msort.c \
	usr.bin/sort/radix_sort.c usr.bin/sort/sort.c usr.bin/sort/tmp.c \
	lib/libutil/efun.c