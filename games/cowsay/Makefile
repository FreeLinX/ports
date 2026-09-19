# FreeLinX/ports - games/cowsay : cowsay 3.7.0 (talking cow generator).
#
# Configurable talking/thinking ASCII cow.
# 100% non-GNU, static Musl libc + Clang build.

NAME    := cowsay
VERSION := 3.7.0
CATEGORY:= games
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= cowsay
INSTALL_RELPATH:= usr/bin/cowsay

include ../../mk/project-port.mk

PROJECT_BIN = $(FREELINX_BUILD_DIR)/work/$(NAME)/cowsay
BUILD_BIN   = $(PROJECT_BIN)

BUILD_CMDS = \
	$(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -static -fuse-ld=lld \
		-o $(PROJECT_BIN) $(FREELINX_ROOT)/games/cowsay/cowsay.c && \
	ln -sf cowsay $(FREELINX_BUILD_DIR)/work/$(NAME)/cowthink
