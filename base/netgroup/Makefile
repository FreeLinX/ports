# FreeLinX/ports - base/netgroup : NetBSD 10.1 `/usr/bin/netgroup`.
NAME   := netgroup
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= netgroup
INSTALL_RELPATH:= bin/netgroup

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/netgroup/netgroup.c

PORT_SRCS = usr.bin/netgroup/netgroup.c

# musl has no netgroup(3) lookup; compat/netgroup.c provides the
# empty/always-no-match implementations (setnetgrent/getnetgrent/endnetgrent).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/netgroup.c
