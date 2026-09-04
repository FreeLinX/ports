# FreeLinX/ports - base/getaddrinfo : NetBSD 10.1 `/usr/bin/getaddrinfo`.
NAME   := getaddrinfo
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= getaddrinfo
INSTALL_RELPATH:= bin/getaddrinfo

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/getaddrinfo/getaddrinfo.c

PORT_SRCS = usr.bin/getaddrinfo/getaddrinfo.c
