# FreeLinX/ports - x11/libxkbcommon : XKB keymap library 1.7.0.
#
# Keymap handling library used by Wayland compositors and X clients.
# Provides xkbcommon for keymap compilation and state tracking.
# X11 and Wayland client features disabled; only the core library is built.
# Static library only.
#
# Build system: Meson + Ninja.

NAME    := libxkbcommon
VERSION := 1.7.0
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl, meson (host), ninja (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

XKBCOMMON_PREFIX = $(FREELINX_BUILD_DIR)/deps/libxkbcommon

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libxkbcommon-$(VERSION)
PROJECT_BIN = $(XKBCOMMON_PREFIX)/lib/libxkbcommon.a
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
		--prefix="$(XKBCOMMON_PREFIX)" \
		-Denable-xkbregistry=false \
		-Denable-docs=false \
		-Denable-tools=false \
		-Denable-x11=false \
		-Denable-wayland=false \
		-Denable-tests=false

BUILD_CMDS = \
	ninja -C flx-build -j"$(FREELINX_JOBS)" && \
	ninja -C flx-build install

.PHONY: install
install:
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@cp -a "$(XKBCOMMON_PREFIX)/lib/"*.a "$(STAGE_ROOT)/usr/lib/"
	@mkdir -p "$(STAGE_ROOT)/usr/include/xkbcommon"
	@cp -a "$(XKBCOMMON_PREFIX)/include/xkbcommon/"*.h "$(STAGE_ROOT)/usr/include/xkbcommon/"
	@printf '[FreeLinX/ports] staged libxkbcommon\n'
