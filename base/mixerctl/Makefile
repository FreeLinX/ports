# FreeLinX/ports - base/mixerctl : NetBSD 10.1 `/usr/bin/mixerctl`.
NAME   := mixerctl
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mixerctl
INSTALL_RELPATH:= bin/mixerctl

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/mixerctl/mixerctl.c

PORT_SRCS = usr.bin/mixerctl/mixerctl.c

# Not portable to a Linux kernel: needs the BSD audio framework (sys/audioio.h, libaudio.h) and proplib
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= needs the BSD audio framework (sys/audioio.h, libaudio.h) and proplib
