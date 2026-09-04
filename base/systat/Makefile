# FreeLinX/ports - base/systat : NetBSD 10.1 `/usr/bin/systat`.
NAME   := systat
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= systat
INSTALL_RELPATH:= bin/systat

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/systat/bufcache.c \
	usr/src/usr.bin/systat/cmds.c \
	usr/src/usr.bin/systat/cmdtab.c \
	usr/src/usr.bin/systat/convtbl.c \
	usr/src/usr.bin/systat/convtbl.h \
	usr/src/usr.bin/systat/df.c \
	usr/src/usr.bin/systat/disks.c \
	usr/src/usr.bin/systat/extern.h \
	usr/src/usr.bin/systat/fetch.c \
	usr/src/usr.bin/systat/globalcmds.c \
	usr/src/usr.bin/systat/icmp.c \
	usr/src/usr.bin/systat/ifcmds.c \
	usr/src/usr.bin/systat/ifstat.c \
	usr/src/usr.bin/systat/iostat.c \
	usr/src/usr.bin/systat/ip.c \
	usr/src/usr.bin/systat/ip6.c \
	usr/src/usr.bin/systat/keyboard.c \
	usr/src/usr.bin/systat/main.c \
	usr/src/usr.bin/systat/mbufs.c \
	usr/src/usr.bin/systat/netcmds.c \
	usr/src/usr.bin/systat/netstat.c \
	usr/src/usr.bin/systat/pigs.c \
	usr/src/usr.bin/systat/ps.c \
	usr/src/usr.bin/systat/ps.h \
	usr/src/usr.bin/systat/swap.c \
	usr/src/usr.bin/systat/syscall.c \
	usr/src/usr.bin/systat/systat.h \
	usr/src/usr.bin/systat/tcp.c \
	usr/src/usr.bin/systat/vmstat.c \
	usr/src/usr.bin/systat/vmstat.h

PORT_SRCS = usr.bin/systat/bufcache.c \
	usr.bin/systat/cmds.c \
	usr.bin/systat/cmdtab.c \
	usr.bin/systat/convtbl.c \
	usr.bin/systat/df.c \
	usr.bin/systat/disks.c \
	usr.bin/systat/fetch.c \
	usr.bin/systat/globalcmds.c \
	usr.bin/systat/icmp.c \
	usr.bin/systat/ifcmds.c \
	usr.bin/systat/ifstat.c \
	usr.bin/systat/iostat.c \
	usr.bin/systat/ip.c \
	usr.bin/systat/ip6.c \
	usr.bin/systat/keyboard.c \
	usr.bin/systat/main.c \
	usr.bin/systat/mbufs.c \
	usr.bin/systat/netcmds.c \
	usr.bin/systat/netstat.c \
	usr.bin/systat/pigs.c \
	usr.bin/systat/ps.c \
	usr.bin/systat/swap.c \
	usr.bin/systat/syscall.c \
	usr.bin/systat/tcp.c \
	usr.bin/systat/vmstat.c
