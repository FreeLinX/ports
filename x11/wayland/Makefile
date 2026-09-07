# FreeLinX/ports - x11/wayland : Wayland 1.23.1 (display protocol library).
#
# The core Wayland protocol library providing libwayland-server and
# libwayland-client.  Built as a static library only (scanner disabled;
# wayland-scanner will be provided by a future host-tool port).  Depends
# on libffi and libxml2 (both future build dependencies).
#
# Build system: Meson + Ninja.  Library-only build (scanner=false).

NAME    := wayland
VERSION := 1.23.1
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libffi
BUILD_DEPENDENCIES:= musl, libffi (future), libxml2 (future), meson (host), ninja (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

WAYLAND_PREFIX = $(FREELINX_BUILD_DIR)/deps/wayland

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/wayland-$(VERSION)
PROJECT_BIN = $(WAYLAND_PREFIX)/lib/libwayland-server.a
BUILD_BIN   = $(PROJECT_BIN)

# Meson cross-compilation file for FreeLinX toolchain.
define MESON_CROSS
[binaries]
c = '$(FREELINX_CC)'
ld = '$(FREELINX_LD)'
ar = '$(FLX_HOST_AR)'
ranlib = '$(FLX_HOST_RANLIB)'
strip = 'true'
[built-in options]
c_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-O2']
c_link_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt']
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
	meson setup flx-build \
		--cross-file=flx-cross.ini \
		--default-library=static \
		--prefix="$(WAYLAND_PREFIX)" \
		-Ddocumentation=false \
		-Ddtd_validation=false \
		-Dtests=false \
		-Dscanner=false

BUILD_CMDS = \
	ninja -C flx-build -j"$(FREELINX_JOBS)" && \
	ninja -C flx-build install

.PHONY: install
install:
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@cp -a "$(WAYLAND_PREFIX)/lib/"*.a "$(STAGE_ROOT)/usr/lib/"
	@mkdir -p "$(STAGE_ROOT)/usr/include/wayland"
	@cp -a "$(WAYLAND_PREFIX)/include/wayland/"*.h "$(STAGE_ROOT)/usr/include/wayland/"
	@printf '[FreeLinX/ports] staged wayland libraries\n'
