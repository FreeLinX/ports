# FreeLinX/ports - base/du : NetBSD 10.1 `/usr/bin/du`.
#
# Builds the ACTUAL NetBSD du source (usr.bin/du/du.c from the 10.1 src set)
# with the FreeLinX toolchain (clang + LLD + musl sysroot), static.  It is not
# an invented du or a host coreutils binary.
#
# libc pieces du pulls in, all extracted from the same src set: fts(3)
# (lib/libc/gen/fts.c + include/fts.h, built the nbtool way) and getbsize(3)
# (lib/libc/gen/getbsize.c) for $BLOCKSIZE (its prototype comes from the
# FreeLinX compat <util.h> stand-in, since musl ships no util.h).
#
# OS porting notes:
#   - humanize_number(3) for -h comes from base/compat (prototype + HN_* flags
#     are declared in the FreeLinX compat <sys/cdefs.h>).
#   - patches/patch-du-st-flags compiles out the BSD-only `-n` (no-dump)
#     check: st_flags/UF_NODUMP do not exist on musl/Linux.
NAME   := du
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/du.  Set before include.
INSTALL_BIN:= du
INSTALL_RELPATH:= bin/du

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/du/du.c \
	usr/src/lib/libc/gen/fts.c usr/src/lib/libc/gen/getbsize.c \
	usr/src/include/fts.h

# fts.c built the nbtool way.
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# musl lacks humanize_number(3); du needs it for `-h` output besides fts(3)'s
# reallocarr(3).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c \
	$(FLX_COMPAT)/humanize_number.c

PORT_SRCS = usr.bin/du/du.c lib/libc/gen/fts.c lib/libc/gen/getbsize.c