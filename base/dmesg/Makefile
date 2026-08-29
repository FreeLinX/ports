# FreeLinX/ports - base/dmesg : FreeLinX-native dmesg (Linux syslog(2)).
#
# NOTE: this is a FreeLinX-native port, NOT the NetBSD dmesg source.  NetBSD's
# dmesg reads the BSD kernel message buffer through libkvm/sysctl (struct
# kern_msgbuf + kvm_read), which has no Linux equivalent.  Instead FreeLinX
# ships a genuine Linux dmesg built on the kernel's syslog(2) ring buffer via
# musl's klogctl(3).  No compat shims are required; see dmesg.c.
#
# Builds with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# Because the FreeLinX kernel is not implemented yet, there is no kernel
# buffer to print: the binary builds/installs, and its error path (klogctl
# failing) is what actually runs until the kernel boots.
#
# IMPORTANT (native-port build): these variables MUST be set BEFORE the
# include below.  base-port.mk expands $(BUILD_BIN) eagerly when it parses its
# `$(BUILD_BIN): do-prepare` rule, so a post-include override would be ignored.

NAME   := dmesg
VERSION:= freelinx
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= dmesg
INSTALL_RELPATH:= bin/dmesg

# FreeLinX-native port: the source lives in this directory, not the NetBSD
# src set.  Point the build at the port's own dir and skip NetBSD extraction.
SRC_DIR = $(CURDIR)
OBJ_DIR = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)
NETBSD_MEMBERS =
PORT_SRCS = dmesg.c
COMPAT_SRCS =

include ../../mk/base-port.mk

# No NetBSD source to extract or patch; do-prepare is a no-op for native ports.
.PHONY: do-prepare
do-prepare:
	@: # FreeLinX-native port: no upstream source to fetch/extract/patch
