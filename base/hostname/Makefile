# FreeLinX/ports - base/hostname : NetBSD 10.1 `/bin/hostname`.
#
# Builds the ACTUAL NetBSD hostname utility source (bin/hostname/hostname.c
# from the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl
# sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() come from base/compat/getprogname.c.
#   - gethostname()/sethostname()/MAXHOSTNAMELEN are POSIX, provided by musl
#     (<sys/param.h> defines MAXHOSTNAMELEN=64, <unistd.h> the calls).
NAME   := hostname
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/hostname -> src/rootfs/bin/hostname.
INSTALL_BIN:= hostname
INSTALL_RELPATH:= bin/hostname

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/hostname/hostname.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = bin/hostname/hostname.c
