# FreeLinX/ports - base/telnet : NetBSD 10.1 `/usr/bin/telnet`.
NAME   := telnet
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= telnet
INSTALL_RELPATH:= bin/telnet

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/telnet/authenc.c \
	usr/src/usr.bin/telnet/commands.c \
	usr/src/usr.bin/telnet/defines.h \
	usr/src/usr.bin/telnet/externs.h \
	usr/src/usr.bin/telnet/general.h \
	usr/src/usr.bin/telnet/main.c \
	usr/src/usr.bin/telnet/network.c \
	usr/src/usr.bin/telnet/ring.c \
	usr/src/usr.bin/telnet/ring.h \
	usr/src/usr.bin/telnet/sys_bsd.c \
	usr/src/usr.bin/telnet/telnet.c \
	usr/src/usr.bin/telnet/terminal.c \
	usr/src/usr.bin/telnet/types.h \
	usr/src/usr.bin/telnet/utilities.c

PORT_SRCS = usr.bin/telnet/authenc.c \
	usr.bin/telnet/commands.c \
	usr.bin/telnet/main.c \
	usr.bin/telnet/network.c \
	usr.bin/telnet/ring.c \
	usr.bin/telnet/sys_bsd.c \
	usr.bin/telnet/telnet.c \
	usr.bin/telnet/terminal.c \
	usr.bin/telnet/utilities.c
