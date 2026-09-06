# FreeLinX/ports - base/tabs : NetBSD 10.1 `/usr/bin/tabs`.
NAME   := tabs
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tabs
INSTALL_RELPATH:= bin/tabs

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tabs/tabs.c

PORT_SRCS = usr.bin/tabs/tabs.c

FLX_CPPFLAGS+= $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD   += $(FLX_NCURSES_LDADD)
