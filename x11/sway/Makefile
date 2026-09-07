# FreeLinX/ports - x11/sway : sway 1.10.1 (Wayland compositor).
#
# Tiling Wayland compositor and replacement for i3.  Requires wlroots
# as its compositor backend library.  Builds sway, swaybar, swaymsg,
# swaynag, and related tools.
#
# Build system: Meson + Ninja.  Depends on wlroots (x11/wlroots).

NAME    := sway
VERSION := 1.10.1
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= wlroots wayland wayland-protocols libxkbcommon
BUILD_DEPENDENCIES:= musl, wlroots (x11/wlroots), wayland (x11/wayland), wayland-protocols (x11/wayland-protocols), libxkbcommon (x11/libxkbcommon), pango (future), json-c (future), libevdev (future), libinput (future), pcre2 (devel/pcre2), meson (host), ninja (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= sway
INSTALL_RELPATH:= usr/bin/sway

WAYLAND_PREFIX   = $(FREELINX_BUILD_DIR)/deps/wayland
WLPROTO_PREFIX   = $(FREELINX_BUILD_DIR)/deps/wayland-protocols
XKBCOMMON_PREFIX = $(FREELINX_BUILD_DIR)/deps/libxkbcommon
WLROOTS_PREFIX   = $(FREELINX_BUILD_DIR)/deps/wlroots
SWAY_PREFIX      = $(FREELINX_BUILD_DIR)/deps/sway

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/sway-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/build/sway/sway
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
c_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-O2', '-I$(WAYLAND_PREFIX)/include', '-I$(WLPROTO_PREFIX)/include', '-I$(XKBCOMMON_PREFIX)/include', '-I$(WLROOTS_PREFIX)/include']
c_link_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt', '-L$(WAYLAND_PREFIX)/lib', '-L$(XKBCOMMON_PREFIX)/lib', '-L$(WLROOTS_PREFIX)/lib']
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
		--prefix="$(SWAY_PREFIX)" \
		-Dswaybar=true \
		-Dswaymsg=true \
		-Dswaynag=true \
		-Dxwayland=disabled \
		-Dtray=disabled \
		-Dman-pages=disabled \
		-Dzsh-completions=disabled \
		-Dbash-completions=disabled \
		-Dfish-completions=disabled \
		-Dtests=false

BUILD_CMDS = \
	ninja -C flx-build -j"$(FREELINX_JOBS)"
