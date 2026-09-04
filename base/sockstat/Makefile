# FreeLinX/ports - base/sockstat : NetBSD 10.1 `/usr/bin/sockstat`.
NAME   := sockstat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= sockstat
INSTALL_RELPATH:= bin/sockstat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/sockstat/prog_ops.h \
	usr/src/usr.bin/sockstat/sockstat.c \
	usr/src/usr.bin/sockstat/sockstat_hostops.c \
	usr/src/usr.bin/sockstat/sockstat_rumpops.c

PORT_SRCS = usr.bin/sockstat/sockstat.c \
	usr.bin/sockstat/sockstat_hostops.c \
	usr.bin/sockstat/sockstat_rumpops.c
