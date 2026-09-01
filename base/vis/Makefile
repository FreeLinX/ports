# FreeLinX/ports - base/vis : NetBSD 10.1 `/usr/bin/vis`.
#
# Builds the ACTUAL NetBSD vis source (usr.bin/vis/{vis.c,foldit.c} plus the
# libc vis(3) implementation lib/libc/gen/vis.c from the 10.1 src set) with
# the FreeLinX toolchain (clang + LLD + musl sysroot), static.  Not an
# invented vis or a host coreutils binary.
#
# libc notes: musl ships no vis(3)/strvis(3), so the libc implementation is
# vendored from the same src set exactly like the ls port vendors it.  vis(3)
# is built the nbtool way; the extracted <vis.h> comes from src set include/.
NAME   := vis
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= vis
INSTALL_RELPATH:= bin/vis

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/vis/vis.c usr/src/usr.bin/vis/foldit.c \
	usr/src/usr.bin/vis/extern.h \
	usr/src/lib/libc/gen/vis.c usr/src/include/vis.h

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/vis/vis.c usr.bin/vis/foldit.c lib/libc/gen/vis.c