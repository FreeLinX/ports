# FreeLinX/ports - base/fmt : NetBSD 10.1 `/usr/bin/fmt`.
NAME   := fmt
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= fmt
INSTALL_RELPATH:= bin/fmt

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/fmt/buffer.h \
	usr/src/usr.bin/fmt/fmt.c

PORT_SRCS = usr.bin/fmt/fmt.c
COMPAT_SRCS += $(FLX_COMPAT)/fgetwln.c
