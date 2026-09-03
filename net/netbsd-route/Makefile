# FreeLinX/ports - net/netbsd-route : NetBSD 10.1 `/sbin/route`.
#
# Upstream source (correct NetBSD 10.1 members, sbin/route). Ported to the
# FreeLinX musl + clang/lld toolchain; static.
#
# PORTING STATUS: SCAFFOLDED. THIS DOES NOT YET BUILD against musl/Linux.
#
# WHY: NetBSD's route is fundamentally a routing-socket client (AF_ROUTE /
# struct rt_msghdr / RTM_ADD, RTM_DELETE, RTM_CHANGE). The Linux kernel has no
# AF_ROUTE; routes are managed via netlink (AF_NETLINK RTM_NEWROUTE). So this
# needs a genuine hostops rewrite (route_hostops.c -> netlink) plus NetBSD
# header shims before it can link. Said work belongs in this port's patches/.
# The scaffold below points at the genuine upstream sources.
NAME   := netbsd-route
VERSION:= 10.1
CATEGORY:= net
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= route
INSTALL_RELPATH:= sbin/route

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/route

NETBSD_MEMBERS = usr/src/sbin/route/route.c usr/src/sbin/route/route_hostops.c \
	usr/src/sbin/route/show.c usr/src/sbin/route/extern.h \
	usr/src/sbin/route/keywords.c usr/src/sbin/route/keywords.h \
	usr/src/sbin/route/prog_ops.h usr/src/sbin/route/rtutil.c \
	usr/src/sbin/route/rtutil.h
PORT_SRCS = sbin/route/route.c sbin/route/route_hostops.c sbin/route/show.c \
	sbin/route/keywords.c sbin/route/rtutil.c

FLX_CPPFLAGS?=-I$(FLX_COMPAT) -I$(SRC_DIR)/include
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c
