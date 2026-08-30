# FreeLinX/ports - base/paste : NetBSD 10.1 `/usr/bin/paste`.
#
# Builds the ACTUAL NetBSD paste utility source (usr.bin/paste/paste.c from
# the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes: none beyond the default compat.  paste.c uses fgetln(3)
# (provided by musl); the default getprogname.o compiles in harmlessly.
NAME   := paste
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/paste -> src/rootfs/bin/paste.
INSTALL_BIN:= paste
INSTALL_RELPATH:= bin/paste

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/paste/paste.c

PORT_SRCS = usr.bin/paste/paste.c