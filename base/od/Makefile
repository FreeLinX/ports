# FreeLinX/ports - base/od : NetBSD 10.1 `/usr/bin/od`.
#
# NetBSD ships /usr/bin/od as the hexdump package's octal/other dump syntax
# driver.  This port builds the ACTUAL NetBSD od source (usr.bin/hexdump/
# {hexdump.c,display.c,odsyntax.c,parse.c,conv.c} from the 10.1 src set)
# with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# It is the sibling of base/hexdump: the two share hexdump.c and differ only
# in the syntax driver (hexsyntax.c vs odsyntax.c).  Not an invented od or a
# host coreutils binary.
NAME   := od
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= od
INSTALL_RELPATH:= bin/od

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/hexdump/hexdump.c usr/src/usr.bin/hexdump/display.c \
	usr/src/usr.bin/hexdump/odsyntax.c usr/src/usr.bin/hexdump/hexsyntax.c \
	usr/src/usr.bin/hexdump/parse.c usr/src/usr.bin/hexdump/conv.c \
	usr/src/usr.bin/hexdump/hexdump.h

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# The COMPAT_SRCS line this port used to carry is gone: getprogname.c and
# easprintf.c are both in mk/base-port.mk's default set, and the third entry,
# base/compat/erealloc.c, is an alternative to estdlib.c - also in the default
# set - rather than an addition to it.  Naming both put emalloc, ecalloc,
# erealloc and estrdup on the link line twice ("duplicate symbol: emalloc").

PORT_SRCS = usr.bin/hexdump/hexdump.c usr.bin/hexdump/display.c \
	usr.bin/hexdump/odsyntax.c usr.bin/hexdump/hexsyntax.c \
	usr.bin/hexdump/parse.c usr.bin/hexdump/conv.c