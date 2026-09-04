# FreeLinX/ports - base/rfcomm_sppd : NetBSD 10.1 `/usr/bin/rfcomm_sppd`.
NAME   := rfcomm_sppd
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rfcomm_sppd
INSTALL_RELPATH:= bin/rfcomm_sppd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/rfcomm_sppd/rfcomm_sppd.c

PORT_SRCS = usr.bin/rfcomm_sppd/rfcomm_sppd.c
