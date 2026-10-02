# FreeLinX/ports - sysutils/pciutils : pciutils 3.13.0 (PCI utilities).
#
# Provides lspci and setpci for PCI bus device identification and
# configuration. Uses make-based build system.

NAME    := pciutils
VERSION := 3.13.0
CATEGORY:= sysutils
LICENSE := GPL-2.0
DEPENDENCIES:= kmod
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), kmod (sysutils/kmod), zlib
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

# sysutils/kmod publishes this prefix; see the stage-libkmod target there for
# why meson's libkmod-internal.a is published as libkmod.a.  pciutils builds
# ls-kernel.o unconditionally and its Makefile appends $(LIBKMOD_CFLAGS) to
# that one object, so the header has to be reachable and the archive has to
# link whether or not anything said --with-kmod.
KMOD_PREFIX = $(FREELINX_BUILD_DIR)/deps/kmod

pciutils_targets = CC="$(FREELINX_CC)" \
	CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -I$(ZLIB_PREFIX)/include -I$(KMOD_PREFIX)/include" \
	LDFLAGS="$(FLX_LINK_FLAGS) -L$(ZLIB_PREFIX)/lib -L$(KMOD_PREFIX)/lib" \
	LDLIBS="-lz -lkmod" \
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
