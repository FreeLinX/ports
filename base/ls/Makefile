# FreeLinX/ports - base/ls : NetBSD 10.1 `/bin/ls`.
#
# Builds the ACTUAL NetBSD ls utility source (bin/ls/{ls.c,main.c,print.c,
# util.c,cmp.c,extern.h,ls.h} from the 10.1 src set) with the FreeLinX
# toolchain (clang + LLD + musl sysroot), static.  It is not an invented ls
# or a host coreutils binary.
#
# libc pieces ls pulls in, all extracted from the same src set: fts(3)
# (lib/libc/gen/fts.c + include/fts.h, built the nbtool way), vis(3)
# (lib/libc/gen/vis.c + include/vis.h) for the byte-vis mode, and
# getbsize(3) (lib/libc/gen/getbsize.c) for $BLOCKSIZE (its prototype comes
# from the FreeLinX compat <util.h> stand-in, since musl ships no util.h).
#
# OS porting notes (see patches/README):
#   - built with -DSMALL (NetBSD small-tools): the ACL code in print.c
#     (<sys/acl.h> + acl_get_link_np...) compiles out.
#   - patches/patch-ls-print-tzfile removes an unused <tzfile.h> include
#     (musl ships no tzfile.h).
#   - patches/patch-ls-flags-linux compiles out the BSD-only `ls -o` file
#     flags column (flags_to_string(3)/st_flags are absent on musl/Linux).
NAME   := ls
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/ls -> src/rootfs/bin/ls.  Set before include.
INSTALL_BIN:= ls
INSTALL_RELPATH:= bin/ls

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/ls/cmp.c usr/src/bin/ls/extern.h \
	usr/src/bin/ls/ls.c usr/src/bin/ls/ls.h \
	usr/src/bin/ls/main.c usr/src/bin/ls/print.c usr/src/bin/ls/util.c \
	usr/src/lib/libc/gen/fts.c usr/src/lib/libc/gen/vis.c \
	usr/src/lib/libc/gen/getbsize.c \
	usr/src/include/fts.h usr/src/include/vis.h

# -DSMALL = NetBSD's own small-tools build; fts.c built the nbtool way.
FLX_CPPFLAGS += -DSMALL -DHAVE_NBTOOL_CONFIG_H=1

# musl lacks strmode(3) (transforms mode to ls-style string) and
# user_from_uid(3)/group_from_gid(3) (long listing); from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strmode.c \
	$(FLX_COMPAT)/user_from_uid.c $(FLX_COMPAT)/reallocarr.c \
	$(FLX_COMPAT)/humanize_number.c

PORT_SRCS = bin/ls/cmp.c bin/ls/ls.c bin/ls/main.c bin/ls/print.c \
	bin/ls/util.c lib/libc/gen/fts.c lib/libc/gen/vis.c \
	lib/libc/gen/getbsize.c