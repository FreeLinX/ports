# FreeLinX/ports - base/man : NetBSD 10.1 `/usr/bin/man`.
NAME   := man
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= man
INSTALL_RELPATH:= bin/man

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/man/man.c \
	usr/src/usr.bin/man/manconf.c \
	usr/src/usr.bin/man/manconf.h \
	usr/src/usr.bin/man/pathnames.h

PORT_SRCS = usr.bin/man/man.c \
	usr.bin/man/manconf.c
