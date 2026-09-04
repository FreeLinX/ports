# FreeLinX/ports - base/rump_dhcpclient : NetBSD 10.1 `/usr/bin/rump_dhcpclient`.
NAME   := rump_dhcpclient
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= rump_dhcpclient
INSTALL_RELPATH:= bin/rump_dhcpclient

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/rump_dhcpclient/bpf-filter.h \
	usr/src/usr.bin/rump_dhcpclient/bpf.c \
	usr/src/usr.bin/rump_dhcpclient/common.c \
	usr/src/usr.bin/rump_dhcpclient/common.h \
	usr/src/usr.bin/rump_dhcpclient/configure.c \
	usr/src/usr.bin/rump_dhcpclient/configure.h \
	usr/src/usr.bin/rump_dhcpclient/dhcp.c \
	usr/src/usr.bin/rump_dhcpclient/dhcp.h \
	usr/src/usr.bin/rump_dhcpclient/dhcpcd.h \
	usr/src/usr.bin/rump_dhcpclient/if-bsd.c \
	usr/src/usr.bin/rump_dhcpclient/if-options.h \
	usr/src/usr.bin/rump_dhcpclient/main.c \
	usr/src/usr.bin/rump_dhcpclient/net.c \
	usr/src/usr.bin/rump_dhcpclient/net.h

PORT_SRCS = usr.bin/rump_dhcpclient/bpf.c \
	usr.bin/rump_dhcpclient/common.c \
	usr.bin/rump_dhcpclient/configure.c \
	usr.bin/rump_dhcpclient/dhcp.c \
	usr.bin/rump_dhcpclient/if-bsd.c \
	usr.bin/rump_dhcpclient/main.c \
	usr.bin/rump_dhcpclient/net.c
