# FreeLinX/ports - base/cp : NetBSD 10.1 `/bin/cp`.
#
# Builds the ACTUAL NetBSD cp utility source (bin/cp/{cp.c,utils.c,extern.h}
# from the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl
# sysroot), static.  It is not an invented cp or a host coreutils binary.
#
# cp walks the tree with NetBSD's fts(3); lib/libc/gen/fts.c + include/fts.h
# are extracted from the same src set and compiled in, with fts.c built the
# NetBSD nbtool way (HAVE_NBTOOL_CONFIG_H defined so the musl struct dirent
# is handled by its portable strlen path).
#
# OS porting notes (see patches/README):
#   - built with -DSMALL, NetBSD's own "small tools" configuration: the ACL
#     preservation code (preserve_fd_acls/preserve_acls, which includes
#     <sys/acl.h> and <sys/extattr.h>) compiles out.  Linux has no POSIX.1e
#     ACLs on files (and musl ships no <sys/acl.h>).
#   - patches/patch-cp-utils-guard-extattr moves the unconditional
#     <sys/extattr.h> include inside the -DSMALL guard (it is only used by
#     the excluded ACL path) and compiles out the BSD-only "copy file flags"
#     step in setfile() (fchflags(2)/st_flags, absent on musl/Linux).
NAME   := cp
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/cp -> src/rootfs/bin/cp.  Set before include.
INSTALL_BIN:= cp
INSTALL_RELPATH:= bin/cp

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/cp/cp.c usr/src/bin/cp/utils.c \
	usr/src/bin/cp/extern.h \
	usr/src/lib/libc/gen/fts.c usr/src/include/fts.h

# -DSMALL = NetBSD's own small-tools build; fts.c built the nbtool way.
FLX_CPPFLAGS += -DSMALL -DHAVE_NBTOOL_CONFIG_H=1

# musl lacks strlcpy(3) and user_from_uid(3)/group_from_gid(3) (cp -i prompt).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c \
	$(FLX_COMPAT)/user_from_uid.c $(FLX_COMPAT)/reallocarr.c

PORT_SRCS = bin/cp/cp.c bin/cp/utils.c lib/libc/gen/fts.c