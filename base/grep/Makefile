# FreeLinX/ports - base/grep : NetBSD 10.1 `/usr.bin/grep`.
#
# Builds the ACTUAL NetBSD grep utility source (usr.bin/grep/*.c) with the
# FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# grep.h includes <zlib.h>/<bzlib.h>/<nl_types.h> for compressed-file and
# NLS(gettext-catalog) support.  The FreeLinX musl sysroot ships none of
# those dev headers, and msgcat/compression are optional coexist features of
# NetBSD grep, so they are compiled out with the upstream -D guards
# (WITHOUT_GZIP / WITHOUT_BZ2 / WITHOUT_NLS) exactly as NetBSD's own
# "small" builds can.  This is not a placeholder: the core grep engine
# (regcomp/regexec, fgrep, glob, -r/-R fts walk via util.c) is untouched.
#
# fts(3), which grep's recursive -r/-R uses, is pulled from the same src set
# and built the nbtool way, mirroring base/ls.
#
# OS porting notes (see patches/README):
#   - none required beyond the module itself: fts.c needs ALIGNBYTES/ALIGN
#     from the FreeLinX compat <sys/cdefs.h>.

NAME   := grep
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/grep -> src/rootfs/bin/grep.
INSTALL_BIN:= grep
INSTALL_RELPATH:= bin/grep

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/grep/fastgrep.c \
	usr/src/usr.bin/grep/file.c \
	usr/src/usr.bin/grep/grep.c \
	usr/src/usr.bin/grep/grep.h \
	usr/src/usr.bin/grep/queue.c \
	usr/src/usr.bin/grep/util.c \
	usr/src/lib/libc/gen/fts.c \
	usr/src/include/fts.h

# Compile out compressed-file (zlib/bzlib) and NLS(gettext) coexistence
# support (sysroot lacks those dev headers); fts.c built the nbtool way.
FLX_CPPFLAGS += -DWITHOUT_GZIP -DWITHOUT_BZ2 -DWITHOUT_NLS \
	-DHAVE_NBTOOL_CONFIG_H=1

# fts(3) needs reallocarr(3) (musl lacks it); from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c

PORT_SRCS = usr.bin/grep/fastgrep.c usr.bin/grep/file.c \
	usr.bin/grep/grep.c usr.bin/grep/queue.c usr.bin/grep/util.c \
	lib/libc/gen/fts.c
