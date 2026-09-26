# FreeLinX/ports - base/hexdump : NetBSD 10.1 `/usr/bin/hexdump`.
#
# Builds the ACTUAL NetBSD hexdump source (usr.bin/hexdump/{hexdump.c,
# display.c,hexsyntax.c,parse.c,conv.c} from the 10.1 src set) with the
# FreeLinX toolchain (clang + LLD + musl sysroot), static.  Not an invented
# hexdump or a host util-linux binary.
#
# libc notes: hexdump is self-contained (no BSD-only libc calls); the od
# variant is the sibling base/od port, built from the same directory using
# odsyntax.c instead of hexsyntax.c.
NAME   := hexdump
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= hexdump
INSTALL_RELPATH:= bin/hexdump

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/hexdump/hexdump.c usr/src/usr.bin/hexdump/display.c \
	usr/src/usr.bin/hexdump/hexsyntax.c usr/src/usr.bin/hexdump/odsyntax.c \
	usr/src/usr.bin/hexdump/parse.c usr/src/usr.bin/hexdump/conv.c \
	usr/src/usr.bin/hexdump/hexdump.h

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# The COMPAT_SRCS line this port used to carry is gone: getprogname.c and
# easprintf.c are both in mk/base-port.mk's default set, and the third entry,
# base/compat/erealloc.c, is an alternative to estdlib.c - also in the default
# set - rather than an addition to it.  Naming both put emalloc, ecalloc,
# erealloc and estrdup on the link line twice ("duplicate symbol: emalloc").

PORT_SRCS = usr.bin/hexdump/hexdump.c usr.bin/hexdump/display.c \
	usr.bin/hexdump/hexsyntax.c usr.bin/hexdump/odsyntax.c \
	usr.bin/hexdump/parse.c usr.bin/hexdump/conv.c