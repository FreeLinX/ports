# FreeLinX/ports - archivers/zstd : zstd 1.5.7 (Zstandard compression).
#
# Facebook's fast lossless compression algorithm. Provides zstd, zstdcat,
# unzstd, zstdmt for .zst compression. Multi-threaded support.

NAME    := zstd
VERSION := 1.5.7
CATEGORY:= archivers
LICENSE := BSD-3-Clause+GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= zstd
INSTALL_RELPATH:= usr/bin/zstd

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/zstd-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/programs/zstd
BUILD_BIN   = $(PROJECT_BIN)

# zstd uses make-based build system with cross-compilation support
PROJECT_CFG_CMDS = \
	$(MAKE) -C $(SRC_TREE)/lib \
		DESTDIR="$(FREELINX_BUILD_DIR)/deps/zstd" \
		prefix=/usr \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		AR="$(FREELINX_TOOLCHAIN_BIN)/llvm-ar" \
		RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
		install

BUILD_CMDS = \
	$(MAKE) -C $(SRC_TREE)/programs \
		DESTDIR="$(FREELINX_BUILD_DIR)/deps/zstd" \
		prefix=/usr \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt" \
		AR="$(FREELINX_TOOLCHAIN_BIN)/llvm-ar" \
		RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
		install
