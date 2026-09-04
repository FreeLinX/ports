# FreeLinX/ports - base/newsyslog : NetBSD 10.1 `/usr/bin/newsyslog`.
NAME   := newsyslog
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= newsyslog
INSTALL_RELPATH:= bin/newsyslog

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/newsyslog/newsyslog.c

PORT_SRCS = usr.bin/newsyslog/newsyslog.c
COMPAT_SRCS += $(FLX_COMPAT)/fparseln.c $(FLX_COMPAT)/signalname.c
