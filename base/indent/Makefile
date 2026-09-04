# FreeLinX/ports - base/indent : NetBSD 10.1 `/usr/bin/indent`.
NAME   := indent
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= indent
INSTALL_RELPATH:= bin/indent

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/indent/args.c \
	usr/src/usr.bin/indent/indent.c \
	usr/src/usr.bin/indent/indent.h \
	usr/src/usr.bin/indent/io.c \
	usr/src/usr.bin/indent/lexi.c \
	usr/src/usr.bin/indent/parse.c \
	usr/src/usr.bin/indent/pr_comment.c

PORT_SRCS = usr.bin/indent/args.c \
	usr.bin/indent/indent.c \
	usr.bin/indent/io.c \
	usr.bin/indent/lexi.c \
	usr.bin/indent/parse.c \
	usr.bin/indent/pr_comment.c
