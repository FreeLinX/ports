# FreeLinX/ports - base/ping : NetBSD 10.1 IPv4 ICMP echo client.
#
# The program uses Linux's raw ICMP socket ABI through musl; it does not need
# a BSD routing socket or a GNU userland component.  Linux capabilities (or
# root) are still required to open a raw ICMP socket.
NAME   := ping
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ping
INSTALL_RELPATH:= bin/ping

include ../../mk/base-port.mk

SRC_DIR   = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR   = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/sbin/ping/ping.c \
	usr/src/sbin/ping/ping_hostops.c \
	usr/src/sbin/ping/prog_ops.h

# ping.c provides its own non-NetBSD getprogname/setprogname fallback; its
# packet identifier still needs the local Linux getrandom-backed arc4random.
COMPAT_SRCS = $(FLX_COMPAT)/arc4random.c
PORT_SRCS = sbin/ping/ping.c sbin/ping/ping_hostops.c

# Exposes clock_gettime(2)/CLOCK_MONOTONIC and musl's BSD typedef aliases.
FLX_CPPFLAGS += -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
