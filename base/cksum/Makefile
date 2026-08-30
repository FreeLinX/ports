# FreeLinX/ports - base/cksum : NetBSD 10.1 `/usr/bin/cksum` (and `sum`).
#
# The POSIX crc32 (`cksum` default) plus `sum` (-o 1 / -o 2) are fully built
# from the upstream cksum.c/crc.c/print.c/sum1.c/sum2.c.  The `-a md5|md4|sha1|
# sha2|rmd160` digest backends are NOT compiled: the NetBSD 10.1 src.tgz set
# omits the libc hash cores (MD5Init/..., sha1c.c, sha2c.c), the *hl.c file
# functions (MD5File/...), and every digest header (<md5.h>/<md4.h>/<sha1.h>/
# <sha2.h>/<rmd160.h>); they cannot be built and are patched out (our patched
# hashes[] table is left empty).  crc.c's BSD <sys/endian.h>/be32dec() are
# replaced by a local big-endian read (musl has neither).  needs getprogname()
# and strlcpy() from compat.
NAME   := cksum
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= cksum
INSTALL_RELPATH:= bin/cksum

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/cksum/cksum.c usr/src/usr.bin/cksum/crc.c \
	usr/src/usr.bin/cksum/crc_extern.h usr/src/usr.bin/cksum/extern.h \
	usr/src/usr.bin/cksum/print.c usr/src/usr.bin/cksum/sum1.c \
	usr/src/usr.bin/cksum/sum2.c

# getprogname(): cksum main dispatch.  strlcpy(): cksum -c filename parsing.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

PORT_SRCS = usr.bin/cksum/cksum.c usr.bin/cksum/crc.c \
	usr.bin/cksum/print.c usr.bin/cksum/sum1.c usr.bin/cksum/sum2.c
