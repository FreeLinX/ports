# FreeLinX/ports - base/calendar : NetBSD 10.1 `/usr/bin/calendar`.
NAME   := calendar
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= calendar
INSTALL_RELPATH:= bin/calendar

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/calendar/calendar.c \
	usr/src/usr.bin/calendar/pathnames.h

PORT_SRCS = usr.bin/calendar/calendar.c
COMPAT_SRCS += $(FLX_COMPAT)/fparseln.c
FLX_CPPFLAGS += -include grp.h
