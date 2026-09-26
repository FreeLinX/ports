# FreeLinX/ports - base/btpin : NetBSD 10.1 `/usr/bin/btpin`.
NAME   := btpin
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= btpin
INSTALL_RELPATH:= bin/btpin

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/btpin/btpin.c

PORT_SRCS = usr.bin/btpin/btpin.c

# Not portable to a Linux kernel: depends on the NetBSD Bluetooth stack (libbluetooth), which FreeLinX does not ship
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= depends on the NetBSD Bluetooth stack (libbluetooth), which FreeLinX does not ship
