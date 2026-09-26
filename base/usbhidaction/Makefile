# FreeLinX/ports - base/usbhidaction : NetBSD 10.1 `/usr/bin/usbhidaction`.
NAME   := usbhidaction
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= usbhidaction
INSTALL_RELPATH:= bin/usbhidaction

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/usbhidaction/usbhidaction.c

PORT_SRCS = usr.bin/usbhidaction/usbhidaction.c

# Not portable to a Linux kernel: uses the NetBSD USB HID stack and its uhid(4) interface
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= uses the NetBSD USB HID stack and its uhid(4) interface
