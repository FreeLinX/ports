# FreeLinX/ports - base/ctags : NetBSD 10.1 `/usr/bin/ctags`.
NAME   := ctags
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ctags
INSTALL_RELPATH:= bin/ctags

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ctags/C.c \
	usr/src/usr.bin/ctags/ctags.c \
	usr/src/usr.bin/ctags/ctags.h \
	usr/src/usr.bin/ctags/fortran.c \
	usr/src/usr.bin/ctags/lisp.c \
	usr/src/usr.bin/ctags/print.c \
	usr/src/usr.bin/ctags/tree.c \
	usr/src/usr.bin/ctags/yacc.c

PORT_SRCS = usr.bin/ctags/C.c \
	usr.bin/ctags/ctags.c \
	usr.bin/ctags/fortran.c \
	usr.bin/ctags/lisp.c \
	usr.bin/ctags/print.c \
	usr.bin/ctags/tree.c \
	usr.bin/ctags/yacc.c
