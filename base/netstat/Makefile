# FreeLinX/ports - base/netstat : NetBSD 10.1 `/usr/bin/netstat`.
NAME   := netstat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= netstat
INSTALL_RELPATH:= bin/netstat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/netstat/atalk.c \
	usr/src/usr.bin/netstat/bpf.c \
	usr/src/usr.bin/netstat/fast_ipsec.c \
	usr/src/usr.bin/netstat/if.c \
	usr/src/usr.bin/netstat/inet.c \
	usr/src/usr.bin/netstat/inet6.c \
	usr/src/usr.bin/netstat/main.c \
	usr/src/usr.bin/netstat/mbuf.c \
	usr/src/usr.bin/netstat/mroute.c \
	usr/src/usr.bin/netstat/mroute6.c \
	usr/src/usr.bin/netstat/netstat.h \
	usr/src/usr.bin/netstat/netstat_hostops.c \
	usr/src/usr.bin/netstat/netstat_rumpops.c \
	usr/src/usr.bin/netstat/pfkey.c \
	usr/src/usr.bin/netstat/pfsync.c \
	usr/src/usr.bin/netstat/route.c \
	usr/src/usr.bin/netstat/unix.c \
	usr/src/usr.bin/netstat/vtw.c \
	usr/src/usr.bin/netstat/vtw.h

PORT_SRCS = usr.bin/netstat/atalk.c \
	usr.bin/netstat/bpf.c \
	usr.bin/netstat/fast_ipsec.c \
	usr.bin/netstat/if.c \
	usr.bin/netstat/inet.c \
	usr.bin/netstat/inet6.c \
	usr.bin/netstat/main.c \
	usr.bin/netstat/mbuf.c \
	usr.bin/netstat/mroute.c \
	usr.bin/netstat/mroute6.c \
	usr.bin/netstat/netstat_hostops.c \
	usr.bin/netstat/netstat_rumpops.c \
	usr.bin/netstat/pfkey.c \
	usr.bin/netstat/pfsync.c \
	usr.bin/netstat/route.c \
	usr.bin/netstat/unix.c \
	usr.bin/netstat/vtw.c
