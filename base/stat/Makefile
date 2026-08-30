# FreeLinX/ports - base/stat : NetBSD 10.1 `/usr.bin/stat`.
#
# Builds the ACTUAL NetBSD stat utility source (usr.bin/stat/stat.c) with the
# FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes (see patches/README):
#   - stat.c depends on the fts(3) functions lstat/fstatat via fts.h; those
#     come from the FreeLinX musl-compat layer (base/compat) which provides
#     the getprogname/setprogname shim.

NAME   := stat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)

TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/stat -> src/rootfs/bin/stat.
INSTALL_BIN:= stat
INSTALL_RELPATH:= bin/stat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# stat.c includes <vis.h> and uses fts(3); pull the NetBSD headers plus the
# libc implementions from the same src set (built the nbtool way) exactly as
# rm/ls do.
NETBSD_MEMBERS = usr/src/usr.bin/stat/stat.c \
	usr/src/lib/libc/gen/vis.c usr/src/include/vis.h \
	usr/src/lib/libc/gen/fts.c usr/src/include/fts.h

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# musl lacks strmode(3) (stat -f %Sp formatting); fts.c needs reallocarr(3).
# Both come from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strmode.c \
	$(FLX_COMPAT)/reallocarr.c

PORT_SRCS = usr.bin/stat/stat.c lib/libc/gen/vis.c lib/libc/gen/fts.c
