# FreeLinX/ports - base/rev : NetBSD 10.1 `/usr/bin/rev`.
NAME   := rev
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /
INSTALL_BIN:= rev
INSTALL_RELPATH:= bin/rev

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# rev.c calls setprogname(3) (default getprogname compat) and fgetwln(3),
# which musl lacks; patches/patch-rev-fgetws rewrites it onto fgetws(3).
NETBSD_MEMBERS = usr/src/usr.bin/rev/rev.c
PORT_SRCS = usr.bin/rev/rev.c
