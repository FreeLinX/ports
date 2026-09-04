# FreeLinX/ports - base/at : NetBSD 10.1 `/usr/bin/at`.
NAME   := at
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= at
INSTALL_RELPATH:= bin/at

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/at/at.c \
	usr/src/usr.bin/at/at.h \
	usr/src/usr.bin/at/panic.c \
	usr/src/usr.bin/at/panic.h \
	usr/src/usr.bin/at/parsetime.c \
	usr/src/usr.bin/at/parsetime.h \
	usr/src/usr.bin/at/pathnames.h \
	usr/src/usr.bin/at/perm.c \
	usr/src/usr.bin/at/perm.h \
	usr/src/usr.bin/at/privs.c \
	usr/src/usr.bin/at/privs.h \
	usr/src/usr.bin/at/stime.c \
	usr/src/usr.bin/at/stime.h

PORT_SRCS = usr.bin/at/at.c \
	usr.bin/at/panic.c \
	usr.bin/at/parsetime.c \
	usr.bin/at/perm.c \
	usr.bin/at/privs.c \
	usr.bin/at/stime.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
