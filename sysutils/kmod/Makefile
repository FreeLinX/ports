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
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

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
c_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-O2', '-I$(ZLIB_PREFIX)/include']
c_link_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt', '-L$(ZLIB_PREFIX)/lib']
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
