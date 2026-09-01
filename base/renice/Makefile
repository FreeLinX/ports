# FreeLinX/ports - base/renice : NetBSD 10.1 `/usr/bin/renice`.
#
# Builds the ACTUAL NetBSD renice source (usr.bin/renice/renice.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented renice or a host procps binary.
#
# libc notes: renice.c uses getpriority(2)/setpriority(2) via <sys/resource.h>
# (musl), err(3), getpwnam(3) and getprogname(3) (base/compat).  No other
# BSD-only APIs.
NAME   := renice
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= renice
INSTALL_RELPATH:= bin/renice

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/renice/renice.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/renice/renice.c