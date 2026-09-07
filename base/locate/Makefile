# FreeLinX/ports - base/locate : NetBSD 10.1 `/usr/bin/locate`.
#
# Builds the ACTUAL NetBSD locate source (usr.bin/locate/locate/locate.c +
# locate.h + pathnames.h from the 10.1 src set) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), static.
#
# OS porting notes:
#   - getprogname()/setprogname() come from base/compat/getprogname.c.
#   - The locate database (created by updatedb/code) is a flat bigram-compressed
#     stream read with fgetc(3)/fread(3) through fastfind(); locate.c itself
#     does NOT use dbopen(3) or the libc db/hash layer, so no <db.h>/db group
#     is pulled in here.  err(3)/errx(3), getopt(3), fnmatch(3) and sys/queue.h
#     (LIST_HEAD for the db list) all come from musl / the FreeLinX compat.
#   - updatedb.sh (POSIX sh in the same usr.bin/locate/locate dir) is not a
#     build product; it is installed by src as /usr/libexec/locate.updatedb.
NAME   := locate
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/usr/bin/locate -> src/rootfs/usr/bin/locate.
INSTALL_BIN:= locate
INSTALL_RELPATH:= usr/bin/locate

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/locate/locate/locate.c \
	usr/src/usr.bin/locate/locate/locate.h \
	usr/src/usr.bin/locate/locate/pathnames.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/locate/locate/locate.c