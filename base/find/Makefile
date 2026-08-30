# FreeLinX/ports - base/find : NetBSD 10.1 `/usr/bin/find`.
#
# Builds the ACTUAL NetBSD find source (usr.bin/find/*.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.  It
# is not an invented find or a host findutils binary.
#
# libc pieces find pulls in, all extracted from the same src set: fts(3)
# (lib/libc/gen/fts.c + include/fts.h, built the nbtool way) and libutil's
# efun.c for emalloc(3) (used by -exec).
#
# OS porting notes (see patches/README in the port doc comment / patches):
#   - <tzfile.h> (musl lacks it) is dropped; netbsd's SECSPERMIN/SECSPERDAY/
#     DAYSPERNYEAR literals and the MNT_* flag defines are supplied locally.
#   - parsedate(3) (musl lacks it) for -asince/-csince/-since/-newermt is
#     replaced in find_parsedate() with epoch-seconds or strptime(3) parsing.
#   - BSD file flags (st_flags/chflags/string_to_flags) compile out: flag
#     guarding is used, and `-flags` becomes an accepted-but-never-matching
#     predicate.
#   - -fstype works for "rdonly" (sb.f_flag & ST_RDONLY); musl's struct
#     statvfs has no f_fstypename and Linux has no MNT_LOCAL, so named types
#     and "local" never match (documented in the patch).
#   - musl lacks S_ISTXT (uses S_ISVTX), SIGINFO (maps to SIGUSR1),
#     __unused, and snprintf_ss(3) in <err.h>.
#   - strmode/user_from_uid/group_from_gid/setmode/getmode/getprogname come
#     from base/compat (declared by the FreeLinX compat <sys/cdefs.h>).
NAME   := find
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/find.  Set before include.
INSTALL_BIN:= find
INSTALL_RELPATH:= bin/find

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/find/extern.h usr/src/usr.bin/find/find.c \
	usr/src/usr.bin/find/find.h usr/src/usr.bin/find/function.c \
	usr/src/usr.bin/find/ls.c usr/src/usr.bin/find/main.c \
	usr/src/usr.bin/find/misc.c usr/src/usr.bin/find/operator.c \
	usr/src/usr.bin/find/option.c \
	usr/src/lib/libc/gen/fts.c \
	usr/src/lib/libutil/efun.c \
	usr/src/include/fts.h

# fts.c built the nbtool way.
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# musl lacks strmode/user_from_uid/group_from_gid/setmode (find: -ls,
# -perm, -nouser, -nogroup) and fts(3) needs reallocarr(3).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strmode.c \
	$(FLX_COMPAT)/user_from_uid.c $(FLX_COMPAT)/setmode.c \
	$(FLX_COMPAT)/reallocarr.c

PORT_SRCS = usr.bin/find/find.c usr.bin/find/function.c usr.bin/find/ls.c \
	usr.bin/find/main.c usr.bin/find/misc.c usr.bin/find/operator.c \
	usr.bin/find/option.c lib/libc/gen/fts.c lib/libutil/efun.c