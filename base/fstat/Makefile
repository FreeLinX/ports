# FreeLinX/ports - base/fstat : NetBSD 10.1 `/usr/bin/fstat`.
NAME   := fstat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= fstat
INSTALL_RELPATH:= bin/fstat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/fstat/fstat.c \
	usr/src/usr.bin/fstat/fstat.h \
	usr/src/usr.bin/fstat/isofs.c \
	usr/src/usr.bin/fstat/misc.c \
	usr/src/usr.bin/fstat/ntfs.c \
	usr/src/usr.bin/fstat/ptyfs.c \
	usr/src/usr.bin/fstat/tmpfs.c \
	usr/src/usr.bin/fstat/zfs.c \
	usr/src/usr.bin/fstat/zfs_znode.h

PORT_SRCS = usr.bin/fstat/fstat.c \
	usr.bin/fstat/isofs.c \
	usr.bin/fstat/misc.c \
	usr.bin/fstat/ntfs.c \
	usr.bin/fstat/ptyfs.c \
	usr.bin/fstat/tmpfs.c \
	usr.bin/fstat/zfs.c
