# FreeLinX/ports - base/touch : NetBSD 10.1 `/usr/bin/touch`.
#
# Builds the ACTUAL NetBSD touch utility source (usr.bin/touch/touch.c from
# the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes (patches/patch-touch-musl):
#   - fixes musl stat-field names: st_atimespec/st_mtimespec -> st_atim/st_mtim.
#   - drops the BSD-only <tzfile.h> and <util.h> includes and supplies
#     DEFFILEMODE + TM_YEAR_BASE (musl ships none of these).
#   - `-d/--date` (parsedate(3)) is reported unsupported; use `-t`/`-r`
#     instead.  The heavy yacc parsedate grammar is not ported.
#   - utimens(2)/lutimens(2) (BSD API, absent from musl) are supplied by
#     base/compat/utimens.c on top of utimensat(2); prototypes are in the
#     compat <sys/stat.h> wrapper.  `-h` acts on the symlink via
#     AT_SYMLINK_NOFOLLOW, matching BSD semantics.
NAME   := touch
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/touch -> src/rootfs/bin/touch.
INSTALL_BIN:= touch
INSTALL_RELPATH:= bin/touch

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/touch/touch.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/utimens.c

PORT_SRCS = usr.bin/touch/touch.c
