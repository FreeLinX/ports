# FreeLinX/ports - base/pkill : NetBSD 10.1 `/usr/bin/pkill`.
NAME   := pkill
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= pkill
INSTALL_RELPATH:= bin/pkill

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/pkill/pkill.c

PORT_SRCS = usr.bin/pkill/pkill.c

# Not portable to a Linux kernel: reads NetBSD kvm/kinfo_proc and BSD sysctl (CTL_VFS, KERN_PROC_*), which Linux does not provide
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= reads NetBSD kvm/kinfo_proc and BSD sysctl (CTL_VFS, KERN_PROC_*), which Linux does not provide
