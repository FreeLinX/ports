# FreeLinX/ports - base/newgrp : NetBSD 10.1 `/usr/bin/newgrp`.
NAME   := newgrp
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= newgrp
INSTALL_RELPATH:= bin/newgrp

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/newgrp/grutil.c \
	usr/src/usr.bin/newgrp/grutil.h \
	usr/src/usr.bin/newgrp/newgrp.c

PORT_SRCS = usr.bin/newgrp/grutil.c \
	usr.bin/newgrp/newgrp.c
