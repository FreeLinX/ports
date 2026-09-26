# FreeLinX/ports - base/sdpquery : NetBSD 10.1 `/usr/bin/sdpquery`.
NAME   := sdpquery
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= sdpquery
INSTALL_RELPATH:= bin/sdpquery

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/sdpquery/command.c \
	usr/src/usr.bin/sdpquery/print.c \
	usr/src/usr.bin/sdpquery/sdpquery.c \
	usr/src/usr.bin/sdpquery/sdpquery.h

PORT_SRCS = usr.bin/sdpquery/command.c \
	usr.bin/sdpquery/print.c \
	usr.bin/sdpquery/sdpquery.c

# Not portable to a Linux kernel: depends on the NetBSD Bluetooth stack (libbluetooth), which FreeLinX does not ship
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= depends on the NetBSD Bluetooth stack (libbluetooth), which FreeLinX does not ship
