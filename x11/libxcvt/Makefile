# FreeLinX/ports - x11/libxcvt : libxcvt 0.1.2.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libxcvt-0.1.2.tar.xz
#
# xorg-server uses this to turn the EDID of a display connector into a mode
# name and a mode list.
#
# The one X.Org library here that is meson rather than autotools, so it does
# not use mk/xorg-lib.mk: there is no configure, no libtool and no .la, and the
# archive is libxcvt.a in the meson build directory rather than src/.libs.
#
# MIT licensed.

NAME    := libxcvt
VERSION := 0.1.2
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), meson (host), ninja (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XCVT_PREFIX = $(FREELINX_BUILD_DIR)/deps/libxcvt

FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/build/flx/lib/libxcvt.a))
BUILD_BIN   = $(PROJECT_LIB)

define MESON_CROSS
[binaries]
c = '$(FREELINX_CC)'
cpp = '$(FREELINX_CXX)'
ar = '/usr/bin/ar'
strip = 'true'
pkgconfig = 'false'
[built-in options]
c_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '$(FREELINX_RESOURCE_FLAGS)', '-O2', '-I$(FREELINX_BUILD_DIR)/deps/xorgproto/include', '-I$(FREELINX_BUILD_DIR)/deps/libX11/include']
c_link_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '$(FREELINX_RESOURCE_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt', '-L$(FREELINX_BUILD_DIR)/deps/libX11/lib']
[host_machine]
system = 'linux'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'
endef
export MESON_CROSS

PROJECT_CFG_CMDS = \
	printf '%s\n' "$$MESON_CROSS" > flx-cross.ini && \
	rm -rf build/flx && \
	meson setup build/flx \
		--cross-file=flx-cross.ini \
		--default-library=static \
		--prefix=/usr

BUILD_CMDS = \
	ninja -C build/flx -j"$(FREELINX_JOBS)"

install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(XCVT_PREFIX)/lib/pkgconfig" "$(XCVT_PREFIX)/include"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(XCVT_PREFIX)/lib/libxcvt.a"
	@cp -R "$(SRC_TREE)/include/." "$(XCVT_PREFIX)/include/"
	@$(call flx-stage-pc,libxcvt,libxcvt)
	@printf '[FreeLinX/ports] staged libxcvt.a into %s\n' "$(XCVT_PREFIX)"
