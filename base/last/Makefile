# FreeLinX/ports - base/last : NetBSD 10.1 `/usr/bin/last`.
#
# Builds the ACTUAL NetBSD last utility source (usr.bin/last/{last.c,want.c}
# from the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl
# sysroot), static.  Not an invented last or a host util-linux binary.
#
# OS porting notes:
#   - `<util.h>` is not in musl; the FreeLinX compat stand-in (base/compat)
#     supplies it.
#   - NetBSD's <tzfile.h> does not exist in musl; patches/patch-last-musl
#     drops the include (last.c uses no timezone-file symbols directly).
#   - want.c is text-included by last.c (not a standalone TU), so it is not
#     compiled on its own.
#   - SUPPORT_UTMPX is left undefined: that path reads the BSD-style
#     _PATH_WTMPX / utmpx records that Linux never writes.  SUPPORT_UTMP
#     reads the real thing (_PATH_WTMP = /var/log/wtmp, struct utmp).
#   - getprogname(3) comes from base/compat.
NAME   := last
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= last
INSTALL_RELPATH:= usr/bin/last

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/last/last.c \
	usr/src/usr.bin/last/want.c

FLX_CPPFLAGS += -DSUPPORT_UTMP

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/last/last.c
