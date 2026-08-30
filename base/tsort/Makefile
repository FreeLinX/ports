# FreeLinX/ports - base/tsort : NetBSD 10.1 `/usr/bin/tsort`.
NAME   := tsort
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /
INSTALL_BIN:= tsort
INSTALL_RELPATH:= bin/tsort

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# tsort.c uses the NetBSD libc db(3) hash access method (db.h/dbopen) plus
# erreci/emalloc/ecalloc and the __UNCONST macro.  musl ships neither db(3)
# nor the libutil allocators, so patches/patch-tsort-musl replaces the db
# hash use with a self-contained string->NODE* hash table and stubs the
# allocators, keeping the topological-sort logic intact.
NETBSD_MEMBERS = usr/src/usr.bin/tsort/tsort.c
PORT_SRCS = usr.bin/tsort/tsort.c
