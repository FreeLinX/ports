# FreeLinX/ports - base/scmdctl : NetBSD 10.1 `/usr/bin/scmdctl`.
NAME   := scmdctl
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= scmdctl
INSTALL_RELPATH:= bin/scmdctl

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/scmdctl/common.c \
	usr/src/usr.bin/scmdctl/common.h \
	usr/src/usr.bin/scmdctl/i2cspi.c \
	usr/src/usr.bin/scmdctl/i2cspi.h \
	usr/src/usr.bin/scmdctl/printscmd.c \
	usr/src/usr.bin/scmdctl/printscmd.h \
	usr/src/usr.bin/scmdctl/responses.h \
	usr/src/usr.bin/scmdctl/scmdctl.c \
	usr/src/usr.bin/scmdctl/scmdctl.h \
	usr/src/usr.bin/scmdctl/scmdctlconst.h \
	usr/src/usr.bin/scmdctl/uart.c \
	usr/src/usr.bin/scmdctl/uart.h

PORT_SRCS = usr.bin/scmdctl/common.c \
	usr.bin/scmdctl/i2cspi.c \
	usr.bin/scmdctl/printscmd.c \
	usr.bin/scmdctl/scmdctl.c \
	usr.bin/scmdctl/uart.c
COMPAT_SRCS += $(FLX_STRTOI_SRCS) $(FLX_SIGSTUB_SRCS)
