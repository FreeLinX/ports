# FreeLinX/ports - base/locale : NetBSD 10.1 `/usr/bin/locale`.
NAME   := locale
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= locale
INSTALL_RELPATH:= bin/locale

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/locale/locale.c \
	usr/src/lib/libc/citrus \
	usr/src/lib/libc/locale

PORT_SRCS = usr.bin/locale/locale.c

# locale.c pulls citrus_namespace.h from libc/citrus.
# The member is extracted under $(SRC_DIR)/lib/libc/citrus, which is not on the
# default include path, so point the compiler at it.
FLX_CPPFLAGS += -I$(SRC_DIR)/lib/libc/citrus

# locale.c also pulls setlocale_local.h from libc/locale.
FLX_CPPFLAGS += -I$(SRC_DIR)/lib/libc/locale

# Not portable to a Linux kernel: reaches into NetBSD libc internals (setlocale_local.h and the citrus chain)
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= reaches into NetBSD libc internals (setlocale_local.h and the citrus chain)
