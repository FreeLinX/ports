# FreeLinX/ports - base/logger : NetBSD 10.1 `/usr/bin/logger`.
#
# Builds the ACTUAL NetBSD logger source (usr.bin/logger/logger.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented logger or a host util-linux binary.
#
# libc notes: logger.c uses syslog(3) (musl has it, talks to /dev/log which
# exists on this Linux), err(3) and getprogname(3) (base/compat).
NAME   := logger
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= logger
INSTALL_RELPATH:= bin/logger

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/logger/logger.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/logger/logger.c