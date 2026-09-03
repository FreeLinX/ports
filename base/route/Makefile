# FreeLinX/ports - base/route : BSD-style IPv4 route utility for Linux.
#
# NetBSD's route command talks to a BSD routing socket, which Linux does not
# implement.  This port preserves the familiar `route` interface while using
# Linux's native route ioctl ABI directly.  No GNU userland is used.
NAME   := route
VERSION:= freelinx
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= route
INSTALL_RELPATH:= sbin/route

include ../../mk/base-port.mk

SRC_DIR   = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR   = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)
NETBSD_MEMBERS =
LOCAL_SRCS = $(CURDIR)/files/route_linux.c
COMPAT_SRCS =
PORT_SRCS = route_linux.c
