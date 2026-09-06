# FreeLinX/ports - base/rlogin : NetBSD 10.1 `/usr/bin/rlogin`.
NAME   := rlogin
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rlogin
INSTALL_RELPATH:= bin/rlogin

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/rlogin/rlogin.c \
	usr/src/usr.bin/rsh/getport.c \
	usr/src/usr.bin/rsh/getport.h

PORT_SRCS = usr.bin/rlogin/rlogin.c \
	usr.bin/rsh/getport.c

FLX_CPPFLAGS+= -I$(SRC_DIR)/usr.bin/rsh

COMPAT_SRCS += $(FLX_COMPAT)/rcmd.c
