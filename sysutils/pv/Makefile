# FreeLinX/ports - sysutils/pv : pv 1.8.13 (pipe viewer).
#
# Displays progress of data through a pipeline. Autoconf build.
# Artistic-2.0 license. Built without NLS.

NAME    := pv
VERSION := 1.8.13
CATEGORY:= sysutils
LICENSE := Artistic-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= pv
INSTALL_RELPATH:= usr/bin/pv

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/pv-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/pv
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		--disable-nls \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
