# FreeLinX/ports - base/cut : NetBSD 10.1 `/usr/bin/cut`.
NAME   := cut
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /
INSTALL_BIN:= cut
INSTALL_RELPATH:= bin/cut

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# cut.c equals cut.c + x_cut.c (x_cut.c is #include'd twice).  It uses
# erealloc(3)/ecalloc(3) and the __unused attribute macro, all NetBSD libutil
# items musl lacks; patches/patch-cut-musl replaces them locally.  fgetln(3)
# and roundup are provided by musl.
# x_cut.c is not a standalone translation unit: cut.c #includes "x_cut.c"
# twice to generate b_cut()/c_cut().  It is extracted as a member so the
# #include resolves, but only cut.c is compiled.
NETBSD_MEMBERS = usr/src/usr.bin/cut/cut.c usr/src/usr.bin/cut/x_cut.c
PORT_SRCS = usr.bin/cut/cut.c
