# FreeLinX/ports - base/ifconfig : BSD-style interface configuration on Linux.
#
# NetBSD ifconfig uses BSD-only socket ioctls and routing structures.  This
# port retains the familiar command interface and maps its IPv4/link subset to
# Linux's native AF_INET interface ioctls, with no GNU userland dependency.
NAME   := ifconfig
VERSION:= freelinx
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ifconfig
INSTALL_RELPATH:= sbin/ifconfig

include ../../mk/base-port.mk

SRC_DIR   = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR   = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)
NETBSD_MEMBERS =
LOCAL_SRCS = $(CURDIR)/files/ifconfig_linux.c
COMPAT_SRCS =
PORT_SRCS = ifconfig_linux.c
