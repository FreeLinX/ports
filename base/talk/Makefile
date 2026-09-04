# FreeLinX/ports - base/talk : NetBSD 10.1 `/usr/bin/talk`.
NAME   := talk
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= talk
INSTALL_RELPATH:= bin/talk

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/talk/ctl.c \
	usr/src/usr.bin/talk/ctl_transact.c \
	usr/src/usr.bin/talk/display.c \
	usr/src/usr.bin/talk/get_addrs.c \
	usr/src/usr.bin/talk/get_names.c \
	usr/src/usr.bin/talk/init_disp.c \
	usr/src/usr.bin/talk/invite.c \
	usr/src/usr.bin/talk/io.c \
	usr/src/usr.bin/talk/look_up.c \
	usr/src/usr.bin/talk/msgs.c \
	usr/src/usr.bin/talk/talk.c \
	usr/src/usr.bin/talk/talk.h \
	usr/src/usr.bin/talk/talk_ctl.h

PORT_SRCS = usr.bin/talk/ctl.c \
	usr.bin/talk/ctl_transact.c \
	usr.bin/talk/display.c \
	usr.bin/talk/get_addrs.c \
	usr.bin/talk/get_names.c \
	usr.bin/talk/init_disp.c \
	usr.bin/talk/invite.c \
	usr.bin/talk/io.c \
	usr.bin/talk/look_up.c \
	usr.bin/talk/msgs.c \
	usr.bin/talk/talk.c
