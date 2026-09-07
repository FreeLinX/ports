# FreeLinX/ports - net/rsync : rsync 3.3.0 (remote file sync).
#
# Fast incremental file transfer utility. Autoconf build, GPL-3.0 license.
# Built without xxhash, zstd, lz4, and openssl compression features.

NAME    := rsync
VERSION := 3.3.0
CATEGORY:= net
LICENSE := GPL-3.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= rsync
INSTALL_RELPATH:= usr/bin/rsync

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/rsync-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/rsync
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		--disable-xxhash \
		--disable-zstd \
		--disable-lz4 \
		--disable-openssl \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
