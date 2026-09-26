# FreeLinX/ports - base/ktruss : NetBSD 10.1 `/usr/bin/ktruss`.
NAME   := ktruss
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ktruss
INSTALL_RELPATH:= bin/ktruss

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ktruss/dump.c

PORT_SRCS = usr.bin/ktruss/dump.c

# Not portable to a Linux kernel: NetBSD kernel tracing (sys/ktrace.h, enum uio_rw) has no Linux analogue
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= NetBSD kernel tracing (sys/ktrace.h, enum uio_rw) has no Linux analogue
