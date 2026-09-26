# FreeLinX/ports - base/eject : NetBSD 10.1 `/usr/bin/eject`.
NAME   := eject
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= eject
INSTALL_RELPATH:= bin/eject

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/eject/am_glue.c \
	usr/src/usr.bin/eject/am_glue.h \
	usr/src/usr.bin/eject/eject.c

PORT_SRCS = usr.bin/eject/am_glue.c \
	usr.bin/eject/eject.c

# Not portable to a Linux kernel: needs NetBSD automount private header am_defs.h, absent from the src set
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= needs NetBSD automount private header am_defs.h, absent from the src set
