# FreeLinX/ports - base/chflags : NetBSD 10.1 `/usr/bin/chflags`.
NAME   := chflags
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= chflags
INSTALL_RELPATH:= bin/chflags

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/chflags/chflags.c

PORT_SRCS = usr.bin/chflags/chflags.c

# Not portable to Linux: see the reason below. Declared rather than left
# to fail - an expected failure should not be reported as a regression, and
# a port that cannot work here should say so instead of looking broken.
PORT_NOT_PORTABLE:= BSD file-flag API: fchflags(2)/lchflags(2) and struct stat's st_flags
