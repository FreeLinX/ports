# FreeLinX/ports - base/df : NetBSD 10.1 `/usr/bin/df`.
#
# Builds the ACTUAL NetBSD df source (usr/src/bin/df/df.c from the 10.1 src set)
# with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes (see patches/README):
#   - musl has statvfs(2) but not BSD getmntinfo(3); df's no-argument
#     "all mounted filesystems" path needs a compat getmntinfo() that reads
#     /proc/self/mounts into a statvfs array.  See base/compat/getmntinfo.c.
#   - musl statvfs is POSIX and its struct layout differs from NetBSD;
#     see base/compat/statvfs.c and base/compat/sys/statvfs.h.
#   - musl ships no util.h; strspct(3) comes from base/compat (strpct.c).
NAME   := df
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= df
INSTALL_RELPATH:= bin/df

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# getbsize.c from the src set (df uses getbsize(3) for $BLOCKSIZE -k default).
NETBSD_MEMBERS = usr/src/bin/df/df.c usr/src/lib/libc/gen/getbsize.c

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# df.c uses PRId64 (from NetBSD's <inttypes.h>) but never includes it
# directly; on NetBSD <sys/param.h> pulls in <inttypes.h>.  musl's param.h
# does not, so force the musl <inttypes.h> in (it defines PRId64 for free).
FLX_CPPFLAGS += -include inttypes.h

# getmntinfo.c, statvfs.c: Linux /proc + statfs to NetBSD struct statvfs.
# strpct.c: strspct(3) percentage formatter used by prtstat() for %-Cap.
# getprogname.c: setprogname()/getprogname() from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/getmntinfo.c \
	$(FLX_COMPAT)/statvfs.c $(FLX_COMPAT)/strpct.c \
	$(FLX_COMPAT)/humanize_number.c

# PORT_SRCS are relative to SRC_DIR after strip-components=2 extraction.
# getbsize.c lives alongside df.c in SRC_DIR because NETBSD_MEMBERS pulls it
# out of usr/src/lib/libc/gen/ and the tar --strip-components=2 rules flatten
# it to lib/libc/gen/getbsize.c inside SRC_DIR.
PORT_SRCS = bin/df/df.c lib/libc/gen/getbsize.c
