# FreeLinX/ports - base/error : NetBSD 10.1 `/usr/bin/error`.
NAME   := error
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= error
INSTALL_RELPATH:= bin/error

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/error/error.h \
	usr/src/usr.bin/error/filter.c \
	usr/src/usr.bin/error/input.c \
	usr/src/usr.bin/error/main.c \
	usr/src/usr.bin/error/pathnames.h \
	usr/src/usr.bin/error/pi.c \
	usr/src/usr.bin/error/subr.c \
	usr/src/usr.bin/error/touch.c

PORT_SRCS = usr.bin/error/filter.c \
	usr.bin/error/input.c \
	usr.bin/error/main.c \
	usr.bin/error/pi.c \
	usr.bin/error/subr.c \
	usr.bin/error/touch.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
