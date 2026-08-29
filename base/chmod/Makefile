# FreeLinX/ports - base/chmod : NetBSD 10.1 `/bin/chmod`.
#
# Builds the ACTUAL NetBSD chmod utility source (bin/chmod/chmod.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# libc pieces chmod pulls in, all extracted from the same src set: fts(3)
# (lib/libc/gen/fts.c + include/fts.h, the recursive walk) and setmode(3)/
# getmode(3) (the symbolic "u=rwx,g=..." mode grammar, from base/compat).
#
# OS porting notes:
#   - setprogname()/getprogname()/setmode()/getmode() come from base/compat.
#   - fts.c is built the nbtool way (-DHAVE_NBTOOL_CONFIG_H=1), exactly as in
#     the base/ls port.
#   - lchmod(2) is declared by musl's <sys/stat.h> (via the compat wrapper's
#     #include_next) and links from musl libc, but Linux has no lchmod
#     syscall: `chmod -h` on a symlink is a documented no-op on FreeLinX.
NAME   := chmod
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/chmod -> src/rootfs/bin/chmod.
INSTALL_BIN:= chmod
INSTALL_RELPATH:= bin/chmod

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/chmod/chmod.c \
	usr/src/lib/libc/gen/fts.c usr/src/include/fts.h

# fts.c built the nbtool way.  (chmod.c itself has no ACL path, so no -DSMALL.)
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/setmode.c \
	$(FLX_COMPAT)/reallocarr.c

PORT_SRCS = bin/chmod/chmod.c lib/libc/gen/fts.c
