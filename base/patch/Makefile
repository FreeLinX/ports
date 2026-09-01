# FreeLinX/ports - base/patch : NetBSD 10.1 `/usr/bin/patch`.
#
# Builds the ACTUAL NetBSD patch utility source (usr.bin/patch/{patch.c,
# backupfile.c,inp.c,mkpath.c,pch.c,util.c} + its .h files, from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# Not an invented patch or a host GNU patch.
#
# OS porting notes:
#   - The upstream headers need <sys/cdefs.h> (__dead/__printflike/__COPYRIGHT);
#     the FreeLinX compat stand-in (base/compat/sys/cdefs.h) supplies it via
#     -I$(FLX_COMPAT).
#   - getprogname/setprogname come from base/compat (musl lacks both).
NAME   := patch
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= patch
INSTALL_RELPATH:= usr/bin/patch

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/patch/patch.c \
	usr/src/usr.bin/patch/backupfile.c usr/src/usr.bin/patch/backupfile.h \
	usr/src/usr.bin/patch/common.h \
	usr/src/usr.bin/patch/inp.c usr/src/usr.bin/patch/inp.h \
	usr/src/usr.bin/patch/mkpath.c \
	usr/src/usr.bin/patch/pathnames.h \
	usr/src/usr.bin/patch/pch.c usr/src/usr.bin/patch/pch.h \
	usr/src/usr.bin/patch/util.c usr/src/usr.bin/patch/util.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/patch/patch.c \
	usr.bin/patch/backupfile.c \
	usr.bin/patch/inp.c \
	usr.bin/patch/mkpath.c \
	usr.bin/patch/pch.c \
	usr.bin/patch/util.c
