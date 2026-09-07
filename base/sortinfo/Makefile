# FreeLinX/ports - base/sortinfo : NetBSD 10.1 `/usr/bin/sortinfo`.
NAME   := sortinfo
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= sortinfo
INSTALL_RELPATH:= bin/sortinfo

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/sortinfo/sortinfo.c

PORT_SRCS = usr.bin/sortinfo/sortinfo.c
# estdlib.c (default COMPAT set) already provides erealloc/ecalloc; adding
# erealloc.c here duplicated emalloc/estrdup/ecalloc/erealloc at link time.
