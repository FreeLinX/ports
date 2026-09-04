# FreeLinX/ports - base/vmstat : NetBSD 10.1 `/usr/bin/vmstat`.
NAME   := vmstat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= vmstat
INSTALL_RELPATH:= bin/vmstat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/vmstat/drvstats.c \
	usr/src/usr.bin/vmstat/drvstats.h \
	usr/src/usr.bin/vmstat/vmstat.c

PORT_SRCS = usr.bin/vmstat/drvstats.c \
	usr.bin/vmstat/vmstat.c
