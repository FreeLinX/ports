# FreeLinX/ports - base/pax : NetBSD 10.1 `/bin/pax`.
#
# Builds the ACTUAL NetBSD source (bin/pax/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := pax
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pax
INSTALL_RELPATH:= bin/pax

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/pax

NETBSD_MEMBERS = usr/src/bin/pax/ar_io.c \
	usr/src/bin/pax/ar_subs.c \
	usr/src/bin/pax/buf_subs.c \
	usr/src/bin/pax/cpio.c \
	usr/src/bin/pax/cpio.h \
	usr/src/bin/pax/dumptar.c \
	usr/src/bin/pax/extern.h \
	usr/src/bin/pax/file_subs.c \
	usr/src/bin/pax/ftree.c \
	usr/src/bin/pax/ftree.h \
	usr/src/bin/pax/gen_subs.c \
	usr/src/bin/pax/getoldopt.c \
	usr/src/bin/pax/options.c \
	usr/src/bin/pax/options.h \
	usr/src/bin/pax/pat_rep.c \
	usr/src/bin/pax/pat_rep.h \
	usr/src/bin/pax/pax.c \
	usr/src/bin/pax/pax.h \
	usr/src/bin/pax/sel_subs.c \
	usr/src/bin/pax/sel_subs.h \
	usr/src/bin/pax/tables.c \
	usr/src/bin/pax/tables.h \
	usr/src/bin/pax/tar.c \
	usr/src/bin/pax/tar.h \
	usr/src/bin/pax/tty_subs.c

PORT_SRCS = bin/pax/ar_io.c \
	bin/pax/ar_subs.c \
	bin/pax/buf_subs.c \
	bin/pax/cpio.c \
	bin/pax/file_subs.c \
	bin/pax/ftree.c \
	bin/pax/gen_subs.c \
	bin/pax/getoldopt.c \
	bin/pax/options.c \
	bin/pax/pat_rep.c \
	bin/pax/pax.c \
	bin/pax/sel_subs.c \
	bin/pax/tables.c \
	bin/pax/tar.c \
	bin/pax/tty_subs.c \
	usr.sbin/mtree/getid.c usr.sbin/mtree/spec.c usr.sbin/mtree/misc.c \
	sbin/mknod/pack_dev.c \
	lib/libc/gen/fts.c
COMPAT_SRCS += $(FLX_COMPAT)/fchrootstub.c
FLX_CPPFLAGS += -include sys/sysmacros.h -DHAVE_SYS_MTIO_H
NETBSD_MEMBERS += usr/src/usr.sbin/mtree/mtree.h
NETBSD_MEMBERS += usr/src/usr.sbin/mtree/getid.c usr/src/usr.sbin/mtree/spec.c \
	usr/src/usr.sbin/mtree/misc.c
NETBSD_MEMBERS += usr/src/sbin/mknod/pack_dev.c
NETBSD_MEMBERS += usr/src/lib/libc/gen/fts.c usr/src/include/fts.h
FLX_CPPFLAGS += -I$(SRC_DIR)/usr.sbin/mtree
COMPAT_SRCS += $(FLX_COMPAT)/parsedate.c
NETBSD_MEMBERS += usr/src/usr.sbin/mtree/extern.h
COMPAT_SRCS += $(FLX_COMPAT)/pwcache.c
NETBSD_MEMBERS += usr/src/sbin/mknod/pack_dev.h
FLX_CPPFLAGS += -I$(SRC_DIR)/sbin/mknod
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1
NETBSD_MEMBERS += usr/src/tools/compat/compat_getopt.h usr/src/include/getopt.h
FLX_CPPFLAGS += -I$(SRC_DIR)/tools/compat -DHAVE_GETOPT_H=0
NETBSD_MEMBERS += usr/src/lib/libc/gen/vis.c usr/src/lib/libc/stdlib/getopt_long.c
COMPAT_SRCS += $(FLX_COMPAT)/strmode.c $(FLX_COMPAT)/user_from_uid.c $(FLX_COMPAT)/reallocarr.c $(FLX_COMPAT)/paxstub.c $(FLX_COMPAT)/fparseln.c $(FLX_COMPAT)/vis.c $(FLX_COMPAT)/unvis.c $(FLX_COMPAT)/setmode.c
PORT_SRCS += lib/libc/stdlib/getopt_long.c
