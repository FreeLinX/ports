# FreeLinX/ports - x11/xorgproto : X.Org protocol headers 2024.1 (header-only port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/proto/xorgproto-2024.1.tar.gz
#
# Not a library and not a program: the headers of every X11 protocol extension,
# under include/X11/extensions/ mostly, plus Xproto.h itself.
#
# It is the single most important port in the X build and the one that replaced
# a dozen others, so this needs saying.
#
#   * xproto 7.0.31 and xextproto 7.3.0 are still published, and every release
#     tarball of libX11, libXext, libXrender and libXfixes asks pkg-config for
#     "xproto >= 7.0.33".  The newest xproto the x.org archive publishes is
#     7.0.31, so no release satisfies it.  xorgproto's own xproto.pc says
#     Version: 7.0.33 -- upstream kept the name and the version so the
#     requirement resolves to the package that succeeded xproto, and it does.
#
#   * The separate *-proto packages -- xextproto, kbproto, renderproto,
#     inputproto, fontsproto, bigreqsproto, dri2proto, presentproto,
#     randrproto, recordproto, resourceproto, saverproto, xcmiscproto,
#     xineramaproto, compositeproto, damageproto, videoproto -- were all merged
#     into xorgproto, and xorgproto ships a .pc for every one of them.  The
#     ports for the individual packages still exist and still stage their own
#     .pc, but xorgproto sorts first in the search path and its copies win,
#     which is the wanted outcome: its headers are the same files, newer.
#
#   * xorgproto's Xfuncproto.h and Xmd.h are newer than xproto 7.0.31's, and
#     Xlib.h wants the newer macros.  So the include order matters and every
#     X.Org library port puts this one first; see mk/xorg-lib.mk.
#
# Headers into build/deps, never into the rootfs, because nothing at runtime
# reads them.

NAME    := xorgproto
VERSION := 2024.1
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
# Nothing is staged into the overlay or the rootfs; see x11/renderproto.
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORGPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/xorgproto

include ../../mk/project-port.mk

# configure is not optional.  It is what turns the 36 <module>.pc.in files into
# <module>.pc: every X.Org library's PKG_CHECK_MODULES resolves a *-proto module
# through them, and without them a build stops at
#
#   Package 'fixesproto' not found
#
# which names the consumer's requirement rather than the absent file.
PROJECT_CFG_CMDS = \
	./configure --prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME)
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging xorgproto headers and %s pkg-config files\n' \
		"$$(ls "$(SRC_TREE)"/*.pc.in 2>/dev/null | wc -l)"
	@mkdir -p "$(XORGPROTO_PREFIX)/include" "$(XORGPROTO_PREFIX)/lib/pkgconfig"
	@cp -R "$(SRC_TREE)/include/." "$(XORGPROTO_PREFIX)/include/"
	@for pc in "$(SRC_TREE)"/*.pc; do \
		test ! -f "$$pc" || $(INSTALL) -m 644 "$$pc" "$(XORGPROTO_PREFIX)/lib/pkgconfig/"; \
	done
	@if [ -z "$(firstword $(wildcard $(XORGPROTO_PREFIX)/lib/pkgconfig/xproto.pc))" ]; then \
		printf '[FreeLinX/ports][error] %s: no pkg-config files were configured\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@printf '[FreeLinX/ports] staged %s headers, %s pkg-config files\n' \
		"$$(find "$(XORGPROTO_PREFIX)/include" -name '*.h' | wc -l)" \
		"$$(ls "$(XORGPROTO_PREFIX)/lib/pkgconfig" | wc -l)"
