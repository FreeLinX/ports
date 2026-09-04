# FreeLinX/ports - base/midirecord : NetBSD 10.1 `/usr/bin/midirecord`.
NAME   := midirecord
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= midirecord
INSTALL_RELPATH:= bin/midirecord

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/midirecord/midirecord.c

PORT_SRCS = usr.bin/midirecord/midirecord.c
