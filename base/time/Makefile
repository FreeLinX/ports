# FreeLinX/ports - base/time : NetBSD 10.1 `/usr/bin/time`.
#
# Builds the ACTUAL NetBSD time utility source (usr.bin/time/{time.c,xtime.c,
# ext.h} from the 10.1 src set) with the FreeLinX toolchain (clang + LLD +
# musl sysroot), static.  Not an invented time or a host GNU time.
#
# xtime.c is a shim that #includes bin/csh/time.c (csh.h universe), so it is
# not compiled here; prusage1() comes from base/compat/prusage1.c instead
# (see that file's header for why).
NAME   := time
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= time
INSTALL_RELPATH:= bin/time

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/time/time.c \
	usr/src/usr.bin/time/ext.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/prusage1.c

PORT_SRCS = usr.bin/time/time.c