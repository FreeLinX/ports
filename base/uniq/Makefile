# FreeLinX/ports - base/uniq : NetBSD 10.1 `/usr/bin/uniq`.
#
# Builds the ACTUAL NetBSD uniq utility source (usr.bin/uniq/uniq.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes: none beyond the default compat.  uniq.c uses
# setprogname(3)/getprogname(3) (base/compat) and fgetln(3), which musl's
# libc provides.
NAME   := uniq
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/uniq -> src/rootfs/bin/uniq.
INSTALL_BIN:= uniq
INSTALL_RELPATH:= bin/uniq

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/uniq/uniq.c

PORT_SRCS = usr.bin/uniq/uniq.c