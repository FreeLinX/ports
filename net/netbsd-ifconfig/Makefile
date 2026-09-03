# FreeLinX/ports - net/netbsd-ifconfig : NetBSD 10.1 `/sbin/ifconfig`.
#
# Upstream source (correct NetBSD 10.1 members, sbin/ifconfig). Ported to the
# FreeLinX musl + clang/lld toolchain; static.
#
# PORTING STATUS: SCAFFOLDED. THIS DOES NOT YET BUILD against musl/Linux.
#
# WHY: NetBSD's ifconfig is built on two Linux-absent kernel interfaces:
#   1. Routing sockets (AF_ROUTE / struct rt_msghdr / RTM_* messages) for
#      interface up/down, address/route changes. Linux uses netlink
#      (AF_NETLINK, RTM_NEWLINK/RTM_NEWADDR) with an entirely different wire
#      protocol.
#   2. NetBSD-only headers and ioctls (<netinet/if_ether.h> layouts, SIOCGIFMEDIA,
#      <sys/sysctl.h> NET_RT_* mibs, KAME/ inet6 defs).
#
# Porting it is therefore a kernel-API adaptation (ifconfig_hostops via
# netlink + Linux ioctls), not a header shim. That work belongs in this port's
# patches/ dir as a NetBSD->Linux hostops layer; until then the port is a
# correct scaffold pointing at the genuine upstream sources.
#
# Upstream members kept for the scaffold (see NetBSD sbin/ifconfig Makefile).
NAME   := netbsd-ifconfig
VERSION:= 10.1
CATEGORY:= net
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ifconfig
INSTALL_RELPATH:= sbin/ifconfig

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/ifconfig

NETBSD_MEMBERS = usr/src/sbin/ifconfig/ifconfig.c usr/src/sbin/ifconfig/af_inet.c \
	usr/src/sbin/ifconfig/af_inet6.c usr/src/sbin/ifconfig/af_inetany.c \
	usr/src/sbin/ifconfig/af_inetany.h usr/src/sbin/ifconfig/af_link.c \
	usr/src/sbin/ifconfig/agr.c usr/src/sbin/ifconfig/carp.c \
	usr/src/sbin/ifconfig/env.c usr/src/sbin/ifconfig/env.h \
	usr/src/sbin/ifconfig/ether.c usr/src/sbin/ifconfig/extern.h \
	usr/src/sbin/ifconfig/ieee80211.c usr/src/sbin/ifconfig/ifconfig_hostops.c \
	usr/src/sbin/ifconfig/l2tp.c usr/src/sbin/ifconfig/lagg.c \
	usr/src/sbin/ifconfig/media.c usr/src/sbin/ifconfig/media.h \
	usr/src/sbin/ifconfig/parse.c usr/src/sbin/ifconfig/parse.h \
	usr/src/sbin/ifconfig/pfsync.c usr/src/sbin/ifconfig/prog_ops.h \
	usr/src/sbin/ifconfig/tunnel.c usr/src/sbin/ifconfig/util.c \
	usr/src/sbin/ifconfig/util.h usr/src/sbin/ifconfig/vlan.c
PORT_SRCS = sbin/ifconfig/ifconfig.c sbin/ifconfig/af_inet.c \
	sbin/ifconfig/af_inet6.c sbin/ifconfig/af_inetany.c sbin/ifconfig/af_link.c \
	sbin/ifconfig/agr.c sbin/ifconfig/carp.c sbin/ifconfig/env.c \
	sbin/ifconfig/ether.c sbin/ifconfig/ieee80211.c sbin/ifconfig/ifconfig_hostops.c \
	sbin/ifconfig/l2tp.c sbin/ifconfig/lagg.c sbin/ifconfig/media.c \
	sbin/ifconfig/parse.c sbin/ifconfig/pfsync.c sbin/ifconfig/tunnel.c \
	sbin/ifconfig/util.c sbin/ifconfig/vlan.c

FLX_CPPFLAGS?=-I$(FLX_COMPAT) -I$(SRC_DIR)/include
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c
