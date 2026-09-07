# FreeLinX/ports - base/pwd : NetBSD 10.1 `/bin/pwd`.
#
# Builds the ACTUAL NetBSD pwd utility source (bin/pwd/pwd.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() come from base/compat/getprogname.c.
#   - getcwd()/stat() are POSIX, provided by musl; getcwd_logical() uses the
#     physical- vs logical-resolution flag via getcwd(3), supported by musl.
NAME   := pwd
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/pwd -> src/rootfs/bin/pwd.
INSTALL_BIN:= pwd
INSTALL_RELPATH:= bin/pwd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/pwd/pwd.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = bin/pwd/pwd.c
