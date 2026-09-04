# FreeLinX/ports - base/login : NetBSD 10.1 `/usr/bin/login`.
NAME   := login
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= login
INSTALL_RELPATH:= bin/login

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/login/common.c \
	usr/src/usr.bin/login/common.h \
	usr/src/usr.bin/login/k5login.c \
	usr/src/usr.bin/login/login.c \
	usr/src/usr.bin/login/login_pam.c \
	usr/src/usr.bin/login/pathnames.h

PORT_SRCS = usr.bin/login/common.c \
	usr.bin/login/k5login.c \
	usr.bin/login/login.c \
	usr.bin/login/login_pam.c
