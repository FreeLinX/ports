# FreeLinX/ports - sysutils/pciutils : pciutils 3.13.0 (PCI utilities).
#
# Provides lspci and setpci for PCI bus device identification and
# configuration. Uses make-based build system.

NAME    := pciutils
VERSION := 3.13.0
CATEGORY:= sysutils
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= lspci
INSTALL_RELPATH:= usr/sbin/lspci

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/pciutils-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/lspci
BUILD_BIN   = $(PROJECT_BIN)

ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

pciutils_targets = CC="$(FREELINX_CC)" \
	CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(ZLIB_PREFIX)/include" \
	LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(ZLIB_PREFIX)/lib" \
	LDLIBS="-lz" \
	AR="$(FREELINX_TOOLCHAIN_BIN)/llvm-ar" \
	RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
	SHARED=no \
	HWDB=no \
	MAN= \
	PKGCONFIG=

PROJECT_CFG_CMDS = \
	$(MAKE) -C $(SRC_TREE) $(pciutils_targets) lspci setpci

BUILD_CMDS = \
	$(MAKE) -C $(SRC_TREE) $(pciutils_targets) lspci setpci
