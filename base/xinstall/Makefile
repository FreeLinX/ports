# FreeLinX/ports - base/xinstall : NetBSD 10.1 `/usr/bin/xinstall`.
NAME   := xinstall
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= xinstall
INSTALL_RELPATH:= bin/xinstall

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/xinstall/pathnames.h \
	usr/src/usr.bin/xinstall/xinstall.c

PORT_SRCS = usr.bin/xinstall/xinstall.c
COMPAT_SRCS += $(FLX_COMPAT)/vis.c
COMPAT_SRCS += $(FLX_COMPAT)/md5c.c $(FLX_COMPAT)/md5hl.c
COMPAT_SRCS += $(FLX_COMPAT)/setmode.c
COMPAT_SRCS += $(FLX_COMPAT)/errc_nw.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
# sigstub.c supplies warnc + uid/gid/flags helpers (xinstall.c needs them).
# errc/vwarnc come from errc_nw.c so only sigstub defines warnc in this link.
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1
