# FreeLinX/ports - base/kill : NetBSD 10.1 `/bin/kill`.
#
# Builds the ACTUAL NetBSD kill utility source (bin/kill/kill.c from the 10.1
# src set, standalone: NOT built with -DSHELL so it excludes the ash-builtin
# code path and its ../../bin/sh/bltin/bltin.h include) with the FreeLinX
# toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() come from base/compat/getprogname.c.
#   - signalname()/signalnumber()/signalnext() come from
#     base/compat/signalname.c (musl <signal.h> lacks all three).
#   - kill(2) and the pid/process-group parsing are plain POSIX.
NAME   := kill
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/kill -> src/rootfs/bin/kill.
INSTALL_BIN:= kill
INSTALL_RELPATH:= bin/kill

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/kill/kill.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/signalname.c

PORT_SRCS = bin/kill/kill.c
