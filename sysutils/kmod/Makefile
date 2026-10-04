# FreeLinX/ports - sysutils/kmod : Linux kernel module tools 34.2.
#
# Real upstream kmod (kernel.org) built with the FreeLinX clang + LLD + musl
# toolchain, fully static.  kmod is a multi-call binary (like busybox): the
# single `kmod` binary is symlinked to modprobe, insmod, rmmod, lsmod,
# modinfo, depmod.  It is the standard Linux kernel module management layer.
#
# Build system: Meson + Ninja (ninja is already ported as base/ninja).
# Dependencies: zlib (optional, already ported as base/zlib).
# Disabled: OpenSSL module signing verification, zstd, xz (add later).
#
# Not NetBSD-derived — this is a Linux-specific system utility required for
# kernel module loading.  No GNU runtime dependency.

NAME   := kmod
VERSION:= 34.2
CATEGORY:= sysutils
LICENSE:= LGPL-2.1-or-later
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl, zlib (base/zlib), meson (host), ninja (host or base/ninja)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Primary binary — the kmod multi-call binary.
INSTALL_BIN:= kmod
INSTALL_RELPATH:= sbin/kmod

# Build-time dependency prefix for zlib.
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

# kmod is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

# Host ar/ranlib.
FLX_HOST_AR      ?= $(FREELINX_TOOLCHAIN_BIN)/llvm-ar
FLX_HOST_RANLIB  ?= $(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib

include ../../mk/project-port.mk

KMOD_BUILD = $(SRC_TREE)/flx-build
PROJECT_BIN = $(KMOD_BUILD)/kmod
BUILD_BIN   = $(PROJECT_BIN)

# Meson cross-compilation file for FreeLinX toolchain.
# Written at configure time into $(SRC_TREE)/flx-cross.ini.
define MESON_CROSS
[binaries]
c = '$(FREELINX_CC)'
ld = '$(FREELINX_LD)'
ar = '$(FLX_HOST_AR)'
ranlib = '$(FLX_HOST_RANLIB)'
strip = 'true'
[built-in options]
c_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '$(FREELINX_RESOURCE_FLAGS)', '-O2', '-I$(ZLIB_PREFIX)/include']
c_link_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '$(FREELINX_RESOURCE_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt', '-L$(ZLIB_PREFIX)/lib']
[host_machine]
system = 'linux'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'
endef
export MESON_CROSS

PROJECT_CFG_CMDS = \
	printf '%s\n' "$$MESON_CROSS" > flx-cross.ini && \
	rm -rf flx-build && \
	sed -i '/^pkg\.generate/,/)/d' meson.build && \
	meson setup flx-build \
		--cross-file=flx-cross.ini \
		--default-library=static \
		--prefix=/ \
		-Dzlib=enabled \
		-Dxz=disabled \
		-Dzstd=disabled \
		-Dopenssl=disabled \
		-Dtools=true \
		-Dmanpages=false \
		-Dlogging=true

BUILD_CMDS = \
	ninja -C flx-build -j"$(FREELINX_JOBS)"

# Multi-binary port: kmod binary is primary (staged by install.mk).
# Symlinks for the multi-call interface are created at install time.
KMOD_LINKS = modprobe insmod rmmod lsmod modinfo depmod

install: $(STAGE_FILE)
	@for link in $(KMOD_LINKS); do \
		printf '[FreeLinX/ports] symlink %s -> kmod\n' "$$link"; \
		ln -sf kmod "$(STAGE_ROOT)/sbin/$$link"; \
	done

install-rootfs: check-install-rootfs $(STAGE_FILE)
	@printf '[FreeLinX/ports] installing %s into rootfs %s\n' "$(NAME)" "$(ROOTFS_FILE)"
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@for link in $(KMOD_LINKS); do \
		ln -sf kmod "$(ROOTFS_DIR)/sbin/$$link"; \
	done

# The same library under its own name, as a build dependency, for
# sysutils/pciutils.
#
# pciutils 3.13 builds ls-kernel.o unconditionally - there is no configure
# switch and no --without-kmod:
#
#   lspci$(EXEEXT): lspci.o ls-vpd.o ls-caps.o ls-caps-vendor.o ls-ecaps.o \
#                   ls-kernel.o ls-tree.o ls-map.o $(COMMON) lib/$(PCIIMPLIB)
#   ls-kernel.o: override CFLAGS+=$(LIBKMOD_CFLAGS)
#
# so ls-kernel.c's `#include <libkmod.h>` has to resolve and the link needs
# kmod_module_new_from_lookup and the rest:
#
#   ls-kernel.c:22:10: fatal error: 'libkmod.h' file not found
#
# meson names the static archive libkmod-internal.a because kmod also builds a
# shared libkmod.so, and with --default-library=static there is no libkmod.a to
# publish.  The archive holds the public API either way:
#
#   $ llvm-nm --defined-only flx-build/libkmod-internal.a | grep ' T kmod_new'
#   0000000000000000 T kmod_new
#   0000000000000000 T kmod_module_new_from_lookup
#
# So publish it as libkmod.a and let a consumer say -lkmod.
#
# Repacked rather than copied, because the meson archive is thin: its members
# are named relative to the build tree (libkmod.so.2.5.1.p/
# libkmod_libkmod-builtin.c.o), and a thin archive only resolves from the
# directory it was made in.  Copied to build/deps it is a list of paths that go
# nowhere, and the consumer's link stops with
#
#   ld.lld: error: .../deps/kmod/lib/libkmod.a: could not get the buffer for a
#   child of the archive: 'libkmod.so.2.5.1.p/libkmod_libkmod-builtin.c.o':
#   No such file or directory
#
# Both meson archives go in, not just libkmod-internal.a.  libshared.a is the
# name meson gives the support objects - hash table, string buffer, path and
# file helpers - that the library calls into but does not own:
#
#   ld.lld: error: undefined symbol: hash_new
#   ld.lld: error: undefined symbol: strbuf_init
#   ld.lld: error: undefined symbol: alias_normalize
#
# and the names of the members show what they are:
#
#   $ llvm-ar t flx-build/libshared.a
#   flx-build/libshared.a.p/shared_hash.c.o
#   flx-build/libshared.a.p/shared_strbuf.c.o
#   flx-build/libshared.a.p/shared_util.c.o
#   flx-build/libshared.a.p/shared_array.c.o
#
# libshared.a is thin as well, and its member paths start with flx-build/, so it
# resolves from $(SRC_TREE) while libkmod-internal.a's start with the archive's
# own directory.  The repack therefore reads each list from the directory that
# archive's members are relative to.
#
# The member list drives the repack, not a glob, so an object the build happens
# to leave next to them is not linked into a library it does not belong to:
#
#   $ llvm-ar t flx-build/libkmod-internal.a
#   libkmod.so.2.5.1.p/libkmod_libkmod-builtin.c.o
#   ...
#
# `ar x` is no help here; both ar implementations refuse it outright:
#
#   /usr/bin/ar: `x' cannot be used on thin archives.
KMOD_PREFIX = $(FREELINX_BUILD_DIR)/deps/kmod
KMOD_UNPACK = $(FREELINX_BUILD_DIR)/obj/kmod-libkmod

.PHONY: stage-libkmod
install: stage-libkmod

stage-libkmod:
	@set -e; \
	$(MKDIR) -p "$(KMOD_PREFIX)/lib" "$(KMOD_PREFIX)/include" "$(KMOD_UNPACK)"; \
	rm -f "$(KMOD_UNPACK)"/*.o "$(KMOD_PREFIX)/lib/libkmod.a"; \
	cd "$(KMOD_BUILD)" && \
		"$(FREELINX_AR)" t libkmod-internal.a | while read -r m; do \
			cp "$$m" "$(KMOD_UNPACK)/"; \
		done; \
	cd "$(SRC_TREE)" && \
		"$(FREELINX_AR)" t "$(KMOD_BUILD)/libshared.a" | while read -r m; do \
			cp "$$m" "$(KMOD_UNPACK)/"; \
		done; \
	cd "$(KMOD_UNPACK)" && \
		"$(FLX_HOST_AR)" rcs "$(KMOD_PREFIX)/lib/libkmod.a" *.o; \
	$(INSTALL) -m 644 "$(SRC_TREE)/libkmod/libkmod.h" \
		"$(KMOD_PREFIX)/include/libkmod.h"; \
	test -s "$(KMOD_PREFIX)/lib/libkmod.a" || { \
		printf '[FreeLinX/ports][error] kmod: libkmod.a came out empty; pciutils would link against nothing\n' >&2; \
		exit 1; \
	}; \
	"$(FREELINX_TOOLCHAIN_BIN)/llvm-nm" --defined-only \
		"$(KMOD_PREFIX)/lib/libkmod.a" >/dev/null 2>&1 || { \
		printf '[FreeLinX/ports][error] kmod: %s is not an archive\n' "$(KMOD_PREFIX)/lib/libkmod.a" >&2; \
		exit 1; \
	}; \
	for sym in kmod_new kmod_module_new_from_lookup hash_new strbuf_init; do \
		"$(FREELINX_TOOLCHAIN_BIN)/llvm-nm" --defined-only \
			"$(KMOD_PREFIX)/lib/libkmod.a" 2>/dev/null | \
			grep -q " T $$sym$$" || { \
			printf '[FreeLinX/ports][error] kmod: %s is missing from the published libkmod.a\n' "$$sym" >&2; \
			exit 1; \
		}; \
	done; \
	printf '[FreeLinX/ports] staged libkmod.a for pciutils\n'
