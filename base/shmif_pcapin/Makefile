# FreeLinX/ports - base/shmif_pcapin : NetBSD 10.1 `/usr/bin/shmif_pcapin`.
NAME   := shmif_pcapin
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= shmif_pcapin
INSTALL_RELPATH:= bin/shmif_pcapin

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/shmif_pcapin/shmif_pcapin.c

PORT_SRCS = usr.bin/shmif_pcapin/shmif_pcapin.c
