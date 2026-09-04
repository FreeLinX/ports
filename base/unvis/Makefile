# FreeLinX/ports - base/unvis : NetBSD 10.1 `/usr/bin/unvis`.
NAME   := unvis
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= unvis
INSTALL_RELPATH:= bin/unvis

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/unvis/unvis.c

PORT_SRCS = usr.bin/unvis/unvis.c
COMPAT_SRCS += $(FLX_COMPAT)/unvis.c
