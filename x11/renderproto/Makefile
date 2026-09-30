# FreeLinX/ports - x11/renderproto : renderproto 0.11.1 (header-only port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/proto/renderproto-0.11.1.tar.gz
#
# Not a library and not a program: the renderproto protocol headers.  libX11's
# configure requires this package by name and stops with "Package requirement
# 'renderproto' could not be satisfied" when it cannot be found, so the port exists
# mostly to satisfy that check and to give libX11 something to compile against.
#
# Same shape as x11/xproto: headers into build/deps, never into the rootfs,
# because nothing at runtime reads them.

NAME    := renderproto
VERSION := 0.11.1
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
# Nothing is staged into the overlay or the rootfs; see x11/libX11.
DEPS_ONLY := yes
LIBRARY_PORT := yes

RENDERPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/renderproto

include ../../mk/project-port.mk

# configure is not optional: some headers in these packages are autoconf config
# headers generated from a .h.in, and copying "*.h" without running configure
# silently stages an incomplete set.
PROJECT_CFG_CMDS = \
	./configure --prefix=/usr
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging renderproto headers\n'
	@mkdir -p "$(RENDERPROTO_PREFIX)/include/X11" "$(RENDERPROTO_PREFIX)/lib/pkgconfig"
	@cp $(SRC_TREE)/*.h "$(RENDERPROTO_PREFIX)/include/X11/"
	@mkdir -p "$(RENDERPROTO_PREFIX)/lib/pkgconfig"
	@printf 'prefix=%s\nexec_prefix=$${prefix}\nincludedir=$${prefix}/include\nincludex11dir=$${prefix}/include/X11\n\nName: renderproto\nDescription: renderproto headers\nVersion: $(VERSION)\nCflags: -I$${includedir}\n' "$(RENDERPROTO_PREFIX)" > "$(RENDERPROTO_PREFIX)/lib/pkgconfig/renderproto.pc"
	@printf '[FreeLinX/ports] staged %s headers\n' "$$(ls "$(RENDERPROTO_PREFIX)/include/X11" | wc -l)"
