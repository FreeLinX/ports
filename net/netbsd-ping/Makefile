# FreeLinX/ports - net/netbsd-ping : NetBSD 10.1 `/sbin/ping`.
#
# Builds the ACTUAL NetBSD 10.1 ping source (sbin/ping) with the FreeLinX
# toolchain (clang + LLD + musl sysroot), static. ICMP echo diagnostics.
#
# PORTING STATUS: SCAFFOLDED, NOT YET BUILDING. NetBSD's ping includes
# NetBSD-kernel-specific headers that musl/Linux do not ship and are not yet
# provided by a FreeLinX compat layer: <netinet/in_systm.h> (n_time/n_short),
# <netinet/ip_var.h>, <netipsec/ipsec.h>, plus BSD <sys/cdefs.h> macros and
# err(3). Unlike base/*, this is not a single ifdef toggle; it needs a small
# FreeLinX patches/ set that maps those headers onto musl/Linux equivalents
# (in_systm -> musl's <netinet/in.h> types, ip_var/ipsec disabled, cdefs/err
# from base/compat). ping_hostops.c is the Linux/host backend (rump ops are
# unused) -- see the NetBSD sbin/ping Makefile.

NAME   := netbsd-ping
VERSION:= 10.1
CATEGORY:= net
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ping
INSTALL_RELPATH:= sbin/ping

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/ping

NETBSD_MEMBERS = usr/src/sbin/ping/ping.c usr/src/sbin/ping/ping_hostops.c \
	usr/src/sbin/ping/prog_ops.h
PORT_SRCS = sbin/ping/ping.c sbin/ping/ping_hostops.c

# NOTE: requires FreeLinX patches/ for NetBSD in_systm/ip_var/ipsec headers
# plus base/compat cdefs + err before this can link.
FLX_CPPFLAGS?=-I$(FLX_COMPAT) -I$(SRC_DIR)/include
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c
