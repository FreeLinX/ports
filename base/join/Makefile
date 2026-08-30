# FreeLinX/ports - base/join : NetBSD 10.1 `/usr/bin/join`.
#
# Builds the ACTUAL NetBSD join utility source (usr.bin/join/join.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes: join.c grows its LINE/INPUT arrays with NetBSD's
# reallocarr(3) (musl omits it; base/compat/reallocarr.c supplies it) and
# uses fgetln(3) (provided by musl) and getprogname(3) (base/compat).
NAME   := join
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/join -> src/rootfs/bin/join.
INSTALL_BIN:= join
INSTALL_RELPATH:= bin/join

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/join/join.c

# musl lacks reallocarr(3) (BSD libc); from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c

PORT_SRCS = usr.bin/join/join.c