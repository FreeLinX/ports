# FreeLinX/ports - base/pmap : NetBSD 10.1 `/usr/bin/pmap`.
NAME   := pmap
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pmap
INSTALL_RELPATH:= bin/pmap

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/pmap/main.c \
	usr/src/usr.bin/pmap/main.h \
	usr/src/usr.bin/pmap/pmap.c \
	usr/src/usr.bin/pmap/pmap.h

PORT_SRCS = usr.bin/pmap/main.c \
	usr.bin/pmap/pmap.c

# Not portable to a Linux kernel: reads NetBSD kvm/kinfo_proc and BSD sysctl (CTL_VFS, KERN_PROC_*), which Linux does not provide
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= reads NetBSD kvm/kinfo_proc and BSD sysctl (CTL_VFS, KERN_PROC_*), which Linux does not provide
