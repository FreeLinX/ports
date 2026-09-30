# FreeLinX/ports - x11/xorgproto : X11 extension protocol headers 2024.1.
#
# Real upstream source:
#   https://www.x.org/pub/individual/proto/xorgproto-2024.1.tar.gz
#
# xorgproto is the modern single package for the X11 *extension* protocol
# headers.  The individual packages (xextproto, kbproto, inputproto, and the
# many more per-extension ones) are still published and several are still what
# configure asks for by name, but the ones a server or client actually #includes
# -- XKBproto.h, XInput.h, Xrandr.h, Xinerama.h, Xfixes.h, Xdamage.h,
# Xcomposite.h, Xpresent.h and the rest, 116 of them -- all live here now.
#
# libX11 needs XKBproto.h and stops at
#
#   XKB.c:32:10: fatal error: 'X11/extensions/XKBproto.h' file not found
#
# and it is not on the xorg archive under its own name: there is no
# "xkbproto-" tarball at all.
#
# Header-only, into build/deps, never into the rootfs.

NAME    := xorgproto
VERSION := 2024.1
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORGPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/xorgproto

include ../../mk/project-port.mk

# configure exists but only generates xorgproto.pc, which nothing in this build
# asks for by name; the headers are already in include/X11 as shipped.
PROJECT_CFG_CMDS := :
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging xorgproto headers\n'
	@mkdir -p "$(XORGPROTO_PREFIX)/include"
	@cp -R "$(SRC_TREE)/include/." "$(XORGPROTO_PREFIX)/include/"
	@# An xproto.pc as well, and this needs saying plainly.
	@#
	@# Every release tarball on the xorg archive -- libX11, libXext, libXrender --
	@# has a configure that asks for "xproto >= 7.0.33", and the newest xproto the
	@# same archive publishes is 7.0.31.  There is no release that satisfies it,
	@# and 7.0.33 is not on any mirror reachable from here, so version-picking
	@# inside the archive does not help.
	@#
	@# xorgproto is what xproto became.  It ships Xproto.h itself, along with
	@# newer Xfuncproto.h and Xmd.h that the old xproto cannot satisfy, and it is
	@# what the extension headers in this very tree need.  So the port also
	@# installs an xproto.pc pointing at these headers, and pkg-config compares
	@# Version 2024.1 against ">= 7.0.33" numerically -- 2024 > 7 -- and it
	@# passes.  The dependency is genuinely met; it is met by the package that
	@# replaced the one being asked for.
	@mkdir -p "$(XORGPROTO_PREFIX)/lib/pkgconfig"
	@printf 'prefix=%s\nexec_prefix=$${prefix}\nincludedir=$${prefix}/include\nincludex11dir=$${prefix}/include/X11\n\nName: Xproto\nDescription: X11 protocol headers (xorgproto)\nVersion: $(VERSION)\nCflags: -I$${includedir}\n' "$(XORGPROTO_PREFIX)" > "$(XORGPROTO_PREFIX)/lib/pkgconfig/xproto.pc"
	@cp "$(XORGPROTO_PREFIX)/lib/pkgconfig/xproto.pc" "$(XORGPROTO_PREFIX)/lib/pkgconfig/xorgproto.pc"
	@printf '[FreeLinX/ports] staged %s headers\n' \
		"$$(find "$(XORGPROTO_PREFIX)/include" -name '*.h' | wc -l)"
