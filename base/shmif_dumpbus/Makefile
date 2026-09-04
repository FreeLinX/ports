# FreeLinX/ports - base/shmif_dumpbus : NetBSD 10.1 `/usr/bin/shmif_dumpbus`.
NAME   := shmif_dumpbus
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= shmif_dumpbus
INSTALL_RELPATH:= bin/shmif_dumpbus

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/shmif_dumpbus/shmif_dumpbus.c

PORT_SRCS = usr.bin/shmif_dumpbus/shmif_dumpbus.c
