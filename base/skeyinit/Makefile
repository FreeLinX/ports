# FreeLinX/ports - base/skeyinit : NetBSD 10.1 `/usr/bin/skeyinit`.
NAME   := skeyinit
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= skeyinit
INSTALL_RELPATH:= bin/skeyinit

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/skeyinit/skeyinit.c

PORT_SRCS = usr.bin/skeyinit/skeyinit.c

# Not portable to a Linux kernel: S/Key one-time-password tools; obsolete and needs the removed skey.h
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= S/Key one-time-password tools; obsolete and needs the removed skey.h
