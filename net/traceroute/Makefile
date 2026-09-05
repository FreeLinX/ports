# FreeLinX/ports - net/traceroute : standalone traceroute.
#
# Simple ICMP/UDP traceroute for musl/FreeLinX.

NAME    := traceroute
VERSION := 1.0
CATEGORY:= net
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= traceroute
INSTALL_RELPATH:= usr/sbin/traceroute

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/traceroute

NETBSD_MEMBERS =

LOCAL_SRCS = $(FREELINX_PORTS_ROOT)/net/traceroute/dist/traceroute-standalone.c
PORT_SRCS = traceroute-standalone.c
