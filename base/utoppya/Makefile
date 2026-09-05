# FreeLinX/ports - base/utoppya : NetBSD 10.1 `/usr/bin/utoppya`.
NAME   := utoppya
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= utoppya
INSTALL_RELPATH:= bin/utoppya

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/utoppya/utoppya.c \
	usr/src/usr.bin/ftp/progressbar.c \
	usr/src/usr.bin/ftp/progressbar.h

PORT_SRCS = usr.bin/utoppya/utoppya.c \
	usr.bin/ftp/progressbar.c

FLX_CPPFLAGS+= -I$(SRC_DIR)/usr.bin/ftp
