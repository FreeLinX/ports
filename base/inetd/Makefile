# FreeLinX/ports - base/inetd : NetBSD 10.1 `/usr.sbin/inetd`.
#
# Builds the ACTUAL NetBSD source (usr.sbin/inetd/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := inetd
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= inetd
INSTALL_RELPATH:= usr/sbin/inetd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/inetd

NETBSD_MEMBERS = usr/src/usr.sbin/inetd/inetd.c \
	usr/src/usr.sbin/inetd/inetd.h \
	usr/src/usr.sbin/inetd/ipsec.c \
	usr/src/usr.sbin/inetd/ipsec.h \
	usr/src/usr.sbin/inetd/parse.c \
	usr/src/usr.sbin/inetd/parse_v2.c \
	usr/src/usr.sbin/inetd/pathnames.h \
	usr/src/usr.sbin/inetd/ratelimit.c

PORT_SRCS = usr.sbin/inetd/inetd.c \
	usr.sbin/inetd/ipsec.c \
	usr.sbin/inetd/parse.c \
	usr.sbin/inetd/parse_v2.c \
	usr.sbin/inetd/ratelimit.c
FLX_CPPFLAGS += -DNO_RPC
COMPAT_SRCS += $(FLX_COMPAT)/pidfile.c
FLX_CPPFLAGS += -DINET6
COMPAT_SRCS += $(FLX_COMPAT)/kqueue.c $(FLX_COMPAT)/strtoi.c $(FLX_COMPAT)/strtou.c
