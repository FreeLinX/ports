# FreeLinX/ports - x11/fontsproto : fontsproto 2.1.3 (header-only port).
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/proto/fontsproto-2.1.3.tar.gz
#
# Not a library and not a program: the font protocol headers, font.h and
# fsmasks.h among them.  libXfont2's configure requires this package by name
# and stops with
#
#   Package requirements (xproto xtrans fontsproto >= 2.1.3 fontenc) were not met
#
# when it cannot be found, so the port exists mostly to satisfy that check and
# to give libXfont2 something to compile against.
#
# Same shape as x11/renderproto: headers into build/deps, never into the rootfs,
# because nothing at runtime reads them.

NAME    := fontsproto
VERSION := 2.1.3
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
# Nothing is staged into the overlay or the rootfs; see x11/renderproto.
DEPS_ONLY := yes
LIBRARY_PORT := yes

FONTSPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/fontsproto

include ../../mk/project-port.mk

# configure is not optional: font.h and friends are config headers generated
# from .h.in, and copying "*.h" without running configure silently stages an
# incomplete set.
PROJECT_CFG_CMDS = \
	./configure --prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME)
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging fontsproto headers\n'
	@mkdir -p "$(FONTSPROTO_PREFIX)/include/X11" "$(FONTSPROTO_PREFIX)/lib/pkgconfig"
	@cp $(SRC_TREE)/*.h "$(FONTSPROTO_PREFIX)/include/X11/"
	@# Single-quoted, so the shell does not expand $${includedir} before sed sees
	@# it and turn it into /include.  A pkg-config file that names a path which
	@# does not exist is worse than none: the consumer compiles against nothing
	@# and the failure is reported against the consumer's own requirement.
	@sed -e 's|@prefix@|$(FONTSPROTO_PREFIX)|' \
	    -e 's|@exec_prefix@|$${prefix}|' \
	    -e 's|@libdir@|$${prefix}/lib|' \
	    -e 's|@includedir@|$${prefix}/include|' \
	    -e 's|@PACKAGE_VERSION@|$(VERSION)|' \
	    "$(SRC_TREE)/fontsproto.pc.in" > "$(FONTSPROTO_PREFIX)/lib/pkgconfig/fontsproto.pc"
	@printf '[FreeLinX/ports] staged %s headers\n' "$$(ls "$(FONTSPROTO_PREFIX)/include/X11" | wc -l)"
