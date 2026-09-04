# FreeLinX/ports - base/write : NetBSD 10.1 `/usr/bin/write`.
NAME   := write
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= write
INSTALL_RELPATH:= bin/write

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/write/term_chk.c \
	usr/src/usr.bin/write/term_chk.h \
	usr/src/usr.bin/write/write.c

PORT_SRCS = usr.bin/write/term_chk.c \
	usr.bin/write/write.c
COMPAT_SRCS += $(FLX_COMPAT)/utmpentry.c $(FLX_COMPAT)/ttymsg.c $(FLX_COMPAT)/strlcpy.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
