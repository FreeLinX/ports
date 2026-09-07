# FreeLinX/ports - x11/wlroots : wlroots 0.18.1 (Wayland compositor library).
#
# Modular Wayland compositor library used by sway and other compositors.
# Provides server-side Wayland protocol implementation, rendering, input
# handling, and output management.  Built as a static library.
#
# Dependencies: wayland, libxkbcommon, libdrm, libgbm, EGL, GLESv2.
# Many of these are future dependencies; the port is structurally correct
# even if some deps are not yet available.
#
# Build system: Meson + Ninja.

NAME    := wlroots
VERSION := 0.18.1
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= wayland libxkbcommon
BUILD_DEPENDENCIES:= musl, wayland (x11/wayland), libxkbcommon (x11/libxkbcommon), libdrm (future), libgbm (future), mesa-egl (future), meson (host), ninja (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

WAYLAND_PREFIX  = $(FREELINX_BUILD_DIR)/deps/wayland
XKBCOMMON_PREFIX = $(FREELINX_BUILD_DIR)/deps/libxkbcommon
WLROOTS_PREFIX  = $(FREELINX_BUILD_DIR)/deps/wlroots

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/wlroots-$(VERSION)
PROJECT_BIN = $(WLROOTS_PREFIX)/lib/libwlroots.a
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
c_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-O2', '-I$(WAYLAND_PREFIX)/include', '-I$(XKBCOMMON_PREFIX)/include']
c_link_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt', '-L$(WAYLAND_PREFIX)/lib', '-L$(XKBCOMMON_PREFIX)/lib']
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
		--prefix="$(WLROOTS_PREFIX)" \
		-Dexamples=false \
		-Dtests=false \
		-Dbackends=wayland \
		-Drenderers=gles2 \
		-Dxwayland=disabled \
		-Dxcb-protocols=disabled \
		-Dlibseat=disabled \
		-Dsession-logind=disabled \
		-Dlogind=disabled \
		-Dsystemd=disabled \
		-Deltanode=disabled \
		-Djpeg=disabled \
		-Dpng=disabled

BUILD_CMDS = \
	ninja -C flx-build -j"$(FREELINX_JOBS)" && \
	ninja -C flx-build install

.PHONY: install
install:
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@cp -a "$(WLROOTS_PREFIX)/lib/"*.a "$(STAGE_ROOT)/usr/lib/"
	@mkdir -p "$(STAGE_ROOT)/usr/include/wlr"
	@cp -a "$(WLROOTS_PREFIX)/include/wlr/"*.h "$(STAGE_ROOT)/usr/include/wlr/" 2>/dev/null || true
	@for d in "$(WLROOTS_PREFIX)/include/wlr/"*/; do \
		if [ -d "$$d" ]; then \
			_dn=$$(basename "$$d"); \
			mkdir -p "$(STAGE_ROOT)/usr/include/wlr/$$_dn"; \
			cp -a "$$d"*.h "$(STAGE_ROOT)/usr/include/wlr/$$_dn/" 2>/dev/null || true; \
		fi; \
	done
	@printf '[FreeLinX/ports] staged wlroots\n'
