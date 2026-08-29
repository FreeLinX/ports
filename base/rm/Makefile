# FreeLinX/ports - base/rm : NetBSD 10.1 `/bin/rm`.
#
# Builds the ACTUAL NetBSD rm utility source (bin/rm/rm.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# It is not an invented rm or a host coreutils binary.
#
# rm walks the tree with NetBSD's fts(3); the libc pieces it pulls in,
# lib/libc/gen/fts.c + include/fts.h, are extracted from the same src set and
# compiled in.  fts.c is built the NetBSD nbtool way: the build defines
# HAVE_NBTOOL_CONFIG_H so fts.c does not declare HAVE_STRUCT_DIRENT_D_NAMLEN
# (musl's struct dirent has no d_namlen) and takes its portable paths.
#
# OS porting notes (see patches/README):
#   - none required beyond the module itself: rm -W's whiteout reference is
#     satisfied by the FreeLinX compat <sys/stat.h> wrapper defining S_IFWHT
#     (Linux never sets that type bit), and fts() needs ALIGNBYTES/ALIGN from
#     the compat <sys/cdefs.h>.
NAME   := rm
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/rm -> src/rootfs/bin/rm.  Set before include.
INSTALL_BIN:= rm
INSTALL_RELPATH:= bin/rm

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/rm/rm.c \
	usr/src/lib/libc/gen/fts.c usr/src/include/fts.h

# fts(3) is compiled the nbtool way so d_namlen stays unclaimed on musl.
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# musl lacks strmode(3) (rm -i prompt), user_from_uid(3)/group_from_gid(3)
# and arc4random(3) (rm -P overwrite pattern); from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strmode.c \
	$(FLX_COMPAT)/user_from_uid.c $(FLX_COMPAT)/arc4random.c \
	$(FLX_COMPAT)/reallocarr.c

PORT_SRCS = bin/rm/rm.c lib/libc/gen/fts.c