# FreeLinX/ports - base/ypmatch : NetBSD 10.1 `/usr/bin/ypmatch`.
NAME   := ypmatch
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ypmatch
INSTALL_RELPATH:= bin/ypmatch

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ypmatch/ypmatch.c

PORT_SRCS = usr.bin/ypmatch/ypmatch.c

# Not portable to a Linux kernel: needs SunRPC/XDR, externalised out of NetBSD 10 and absent from the FreeLinX tree
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= needs SunRPC/XDR, externalised out of NetBSD 10 and absent from the FreeLinX tree
