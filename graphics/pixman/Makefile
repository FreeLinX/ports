# FreeLinX/ports - graphics/pixman : pixel manipulation library 0.43.4.
#
# Real upstream source:
#   https://www.cairographics.org/releases/pixman-0.43.4.tar.gz
#
# Low-level pixel manipulation: compositing and blitting on 8- and 16-bit
# channels.  xorg-server links it and asks pkg-config for "pixman-1", so the
# module name is pixman-1.pc even though the library is libpixman-1.
#
# MIT licensed.  Meson build -- the tarball ships no configure, so the port's
# ./configure step failed at
#
#     /bin/sh: line 2: ./configure: No such file or directory
#
# which reads as a missing toolchain rather than a missing build system.
#
# --prefix is the staged tree and not /usr, for the reason mk/xorg-lib.mk gives:
# meson writes it into pixman-1.pc, and a .pc whose prefix is /usr has its
# Cflags dropped by pkg-config as a system path.
#
# Staged into build/deps like every other library, not into the rootfs overlay:
# nothing at runtime loads it, and a library in the overlay has no pkg-config
# file, so xorg-server's configure would stop with "Package 'pixman-1' not
# found".

NAME    := pixman
VERSION := 0.43.4
CATEGORY:= graphics
LICENSE := MIT
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), meson (host), ninja (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_PC_NAME := pixman-1

PIXMAN_PREFIX = $(FREELINX_BUILD_DIR)/deps/pixman

FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/build/flx/pixman/libpixman-1.a))
BUILD_BIN   = $(PROJECT_LIB)

define MESON_CROSS
[binaries]
c = '$(FREELINX_CC)'
cpp = '$(FREELINX_CXX)'
ar = '/usr/bin/ar'
strip = 'true'
pkgconfig = 'false'
[built-in options]
c_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '$(FREELINX_RESOURCE_FLAGS)', '-O2']
c_link_args = ['$(FREELINX_TARGET_FLAGS)', '$(FREELINX_SYSROOT_FLAGS)', '$(FREELINX_RESOURCE_FLAGS)', '-static', '-fuse-ld=lld', '--rtlib=compiler-rt']
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
		--prefix="$(PIXMAN_PREFIX)" \
		-Dtests=disabled \
		-Ddemos=disabled \
		-Dgtk=disabled \
		-Dlibpng=disabled \
		-Dtls=disabled

BUILD_CMDS = \
	ninja -C build/flx -j"$(FREELINX_JOBS)"

install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(PIXMAN_PREFIX)/lib/pkgconfig" "$(PIXMAN_PREFIX)/include/pixman-1"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(PIXMAN_PREFIX)/lib/libpixman-1.a"
	@# The SIMD variants are separate archives that libpixman-1.a's objects
	@# reference, so a --static consumer needs them staged beside it.
	@for e in $(SRC_TREE)/build/flx/pixman/libpixman-*.a; do \
		case "$$e" in *libpixman-1.a) continue ;; esac; \
		$(INSTALL) -m 644 "$$e" "$(PIXMAN_PREFIX)/lib/"; \
	done
	@# The generated headers come from the build directory, not the source tree:
	@# pixman.h does #include <pixman-version.h> and pixman-features.h is
	@# generated too, so copying the source tree alone leaves a tree where every
	@# header a consumer names is present and the build still stops at
	@#   pixman.h:72:10: fatal error: 'pixman-version.h' file not found
	@cp -R "$(SRC_TREE)/pixman/." "$(PIXMAN_PREFIX)/include/pixman-1/"
	@for h in pixman-version.h pixman-features.h; do \
		test ! -f "$(SRC_TREE)/build/flx/pixman/$$h" || \
			$(INSTALL) -m 644 "$(SRC_TREE)/build/flx/pixman/$$h" "$(PIXMAN_PREFIX)/include/pixman-1/"; \
	done
	@if [ ! -f "$(PIXMAN_PREFIX)/include/pixman-1/pixman-version.h" ]; then \
		printf '[FreeLinX/ports][error] %s: pixman-version.h was not generated\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@$(call flx-stage-pc,pixman,pixman-1)
	@printf '[FreeLinX/ports] staged libpixman-1.a into %s\n' "$(PIXMAN_PREFIX)"
