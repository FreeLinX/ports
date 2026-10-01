# FreeLinX/ports - x11/kbproto : kbproto 1.0.7 (header-only port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/proto/kbproto-1.0.7.tar.gz
#
# Not a library and not a program: the kbproto protocol headers.  libX11's
# configure requires this package by name and stops with "Package requirement
# 'kbproto' could not be satisfied" when it cannot be found, so the port exists
# mostly to satisfy that check and to give libX11 something to compile against.
#
# Same shape as x11/xproto: headers into build/deps, never into the rootfs,
# because nothing at runtime reads them.

NAME    := kbproto
VERSION := 1.0.7
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
# Nothing is staged into the overlay or the rootfs; see x11/libX11.
DEPS_ONLY := yes
LIBRARY_PORT := yes

KBPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/kbproto

include ../../mk/project-port.mk

# configure is not optional: some headers in these packages are autoconf config
# headers generated from a .h.in, and copying "*.h" without running configure
# silently stages an incomplete set.
PROJECT_CFG_CMDS = \
	./configure --prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME)
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging kbproto headers\n'
	@mkdir -p "$(KBPROTO_PREFIX)/include/X11" "$(KBPROTO_PREFIX)/lib/pkgconfig"
	@cp $(SRC_TREE)/*.h "$(KBPROTO_PREFIX)/include/X11/"
	@mkdir -p "$(KBPROTO_PREFIX)/lib/pkgconfig"
	@printf 'prefix=%s\nexec_prefix=$${prefix}\nincludedir=$${prefix}/include\nincludex11dir=$${prefix}/include/X11\n\nName: kbproto\nDescription: kbproto headers\nVersion: $(VERSION)\nCflags: -I$${includedir}\n' "$(KBPROTO_PREFIX)" > "$(KBPROTO_PREFIX)/lib/pkgconfig/kbproto.pc"
	@printf '[FreeLinX/ports] staged %s headers\n' "$$(ls "$(KBPROTO_PREFIX)/include/X11" | wc -l)"
