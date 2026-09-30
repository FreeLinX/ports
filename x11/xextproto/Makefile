# FreeLinX/ports - x11/xextproto : X11 extension protocol headers 7.3.0.
#
# Real upstream source:
#   https://www.x.org/pub/individual/proto/xextproto-7.3.0.tar.gz
#
# Not a library and not a program: the headers that define the X11 *extension*
# protocols (Xext.h, Xdamage.h, Xfixes.h, XInput.h, Xrandr.h, Xinerama.h,
# Xcomposite.h, ...).  libX11's configure requires xextproto explicitly, and
# libXext needs X11/Xext.h to compile at all.  Same shape as xproto: headers
# into build/deps, never into the rootfs, because nothing at runtime reads them.

NAME    := xextproto
VERSION := 7.3.0
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
# Nothing is staged into the overlay or the rootfs; see x11/libX11.
DEPS_ONLY := yes
LIBRARY_PORT := yes

XEXTPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/xextproto

include ../../mk/project-port.mk

# Nothing to configure and nothing to compile: the tarball is a directory of
# headers, installed below into build/deps.
PROJECT_CFG_CMDS := :
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging xextproto headers\n'
	@mkdir -p "$(XEXTPROTO_PREFIX)/include/X11"
	@cp $(SRC_TREE)/*.h "$(XEXTPROTO_PREFIX)/include/X11/"
	@printf '[FreeLinX/ports] staged %s headers\n' "$$(ls "$(XEXTPROTO_PREFIX)/include/X11" | wc -l)"
	@# xextproto.pc, for the same reason as xproto.pc: libX11's configure asks
	@# pkg-config for "xproto >= 7.0.25 xextproto xtrans xcb >= 1.11.1".
	@mkdir -p "$(XEXTPROTO_PREFIX)/lib/pkgconfig"
	@printf 'prefix=%s\nexec_prefix=$${prefix}\nincludedir=$${prefix}/include\nincludex11dir=$${prefix}/include/X11\n\nName: Xextproto\nDescription: Xext protocol headers\nVersion: $(VERSION)\nCflags: -I$${includedir}\n' "$(XEXTPROTO_PREFIX)" > "$(XEXTPROTO_PREFIX)/lib/pkgconfig/xextproto.pc"
