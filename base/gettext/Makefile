# FreeLinX/ports - base/gettext : NetBSD 10.1 `/usr/bin/gettext`.
NAME   := gettext
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= gettext
INSTALL_RELPATH:= bin/gettext

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/gettext/gettext.c

PORT_SRCS = usr.bin/gettext/gettext.c
# estdlib.c (default COMPAT set) already provides emalloc/estrdup; adding
# emalloc.c here duplicated those symbols at link time.
