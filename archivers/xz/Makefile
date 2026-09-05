# FreeLinX/ports - archivers/xz : xz-utils 5.6.4 (xz/lzma compression).
#
# Provides xz, xzcat, lzma, unxz, unlzma for .xz and .lzma compression.

NAME    := xz
VERSION := 5.6.4
CATEGORY:= archivers
LICENSE := BSD-0/PD+GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= xz
INSTALL_RELPATH:= usr/bin/xz

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/xz-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/xz/xz
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--enable-static \
		--disable-shared \
		--disable-xzdec \
		--disable-lzmadec \
		--disable-lzmainfo \
		--disable-scripts \
		--disable-doc \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
