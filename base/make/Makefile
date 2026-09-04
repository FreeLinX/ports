# FreeLinX/ports - base/make : NetBSD 10.1 `/usr/bin/make`.
NAME   := make
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= make
INSTALL_RELPATH:= bin/make

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/make/arch.c \
	usr/src/usr.bin/make/buf.c \
	usr/src/usr.bin/make/buf.h \
	usr/src/usr.bin/make/compat.c \
	usr/src/usr.bin/make/cond.c \
	usr/src/usr.bin/make/config.h \
	usr/src/usr.bin/make/dir.c \
	usr/src/usr.bin/make/dir.h \
	usr/src/usr.bin/make/for.c \
	usr/src/usr.bin/make/hash.c \
	usr/src/usr.bin/make/hash.h \
	usr/src/usr.bin/make/job.c \
	usr/src/usr.bin/make/job.h \
	usr/src/usr.bin/make/lst.c \
	usr/src/usr.bin/make/lst.h \
	usr/src/usr.bin/make/main.c \
	usr/src/usr.bin/make/make.c \
	usr/src/usr.bin/make/make.h \
	usr/src/usr.bin/make/make_malloc.c \
	usr/src/usr.bin/make/make_malloc.h \
	usr/src/usr.bin/make/meta.c \
	usr/src/usr.bin/make/meta.h \
	usr/src/usr.bin/make/metachar.c \
	usr/src/usr.bin/make/metachar.h \
	usr/src/usr.bin/make/parse.c \
	usr/src/usr.bin/make/pathnames.h \
	usr/src/usr.bin/make/str.c \
	usr/src/usr.bin/make/str.h \
	usr/src/usr.bin/make/suff.c \
	usr/src/usr.bin/make/targ.c \
	usr/src/usr.bin/make/trace.c \
	usr/src/usr.bin/make/trace.h \
	usr/src/usr.bin/make/util.c \
	usr/src/usr.bin/make/var.c

PORT_SRCS = usr.bin/make/arch.c \
	usr.bin/make/buf.c \
	usr.bin/make/compat.c \
	usr.bin/make/cond.c \
	usr.bin/make/dir.c \
	usr.bin/make/for.c \
	usr.bin/make/hash.c \
	usr.bin/make/job.c \
	usr.bin/make/lst.c \
	usr.bin/make/main.c \
	usr.bin/make/make.c \
	usr.bin/make/make_malloc.c \
	usr.bin/make/meta.c \
	usr.bin/make/metachar.c \
	usr.bin/make/parse.c \
	usr.bin/make/str.c \
	usr.bin/make/suff.c \
	usr.bin/make/targ.c \
	usr.bin/make/trace.c \
	usr.bin/make/util.c \
	usr.bin/make/var.c
