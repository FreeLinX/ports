# FreeLinX/ports - x11/inputproto : inputproto 2.3.2 (header-only port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/proto/inputproto-2.3.2.tar.gz
#
# Not a library and not a program: the inputproto protocol headers.  libX11's
# configure requires this package by name and stops with "Package requirement
# 'inputproto' could not be satisfied" when it cannot be found, so the port exists
# mostly to satisfy that check and to give libX11 something to compile against.
#
# Same shape as x11/xproto: headers into build/deps, never into the rootfs,
# because nothing at runtime reads them.

NAME    := inputproto
VERSION := 2.3.2
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
# Nothing is staged into the overlay or the rootfs; see x11/libX11.
DEPS_ONLY := yes
LIBRARY_PORT := yes

INPUTPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/inputproto

include ../../mk/project-port.mk

# configure is not optional: some headers in these packages are autoconf config
# headers generated from a .h.in, and copying "*.h" without running configure
# silently stages an incomplete set.
PROJECT_CFG_CMDS = \
	./configure --prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME)
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging inputproto headers\n'
	@mkdir -p "$(INPUTPROTO_PREFIX)/include/X11" "$(INPUTPROTO_PREFIX)/lib/pkgconfig"
	# include/X11/extensions/, because that is where libXtst looks: its
	# XTest.h does #include <X11/extensions/XInput.h>.  A flat copy has every
	# header present and still fails there.
	@mkdir -p "$(INPUTPROTO_PREFIX)/include/X11/extensions"
	@cp $(SRC_TREE)/*.h "$(INPUTPROTO_PREFIX)/include/X11/extensions/"
	@mkdir -p "$(INPUTPROTO_PREFIX)/lib/pkgconfig"
	@printf 'prefix=%s\nexec_prefix=$${prefix}\nincludedir=$${prefix}/include\nincludex11dir=$${prefix}/include/X11/extensions\n\nName: inputproto\nDescription: inputproto headers\nVersion: $(VERSION)\nCflags: -I$${includedir}\n' "$(INPUTPROTO_PREFIX)" > "$(INPUTPROTO_PREFIX)/lib/pkgconfig/inputproto.pc"
	@printf '[FreeLinX/ports] staged %s headers\n' "$$(ls "$(INPUTPROTO_PREFIX)/include/X11/extensions" | wc -l)"
