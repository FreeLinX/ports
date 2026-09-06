# FreeLinX/ports - base/progress : NetBSD 10.1 `/usr/bin/progress`.
NAME   := progress
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= progress
INSTALL_RELPATH:= bin/progress

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/progress/progress.c \
	usr/src/usr.bin/ftp/progressbar.c \
	usr/src/usr.bin/ftp/progressbar.h

PORT_SRCS = usr.bin/progress/progress.c \
	usr.bin/ftp/progressbar.c

FLX_CPPFLAGS+= -I$(SRC_DIR)/usr.bin/ftp

COMPAT_SRCS += $(FLX_STRCPYTRICK_SRCS)
