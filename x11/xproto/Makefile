# FreeLinX/ports - x11/xproto : X11 protocol headers 7.0.31 (header-only port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/proto/xproto-7.0.31.tar.gz
#
# Not a library and not a program: this is the set of headers that define the
# core X11 protocol (Xproto.h, Xatom.h, Xfuncproto.h, Xos.h, keysymdef.h, ...).
# Every other X library needs them and none of them ship their own copy:
#
#   libXdmcp:  fatal error: 'X11/Xos.h' file not found
#   libXau:    fatal error: 'X11/Xfuncproto.h' file not found
#   libxcb:    fatal error: 'X11/Xauth.h' file not found
#   libX11:    configure: error: Package requirements (xproto >= 7.0.25 ...) unmet
#
# It is a header-only data port, so it stages into build/deps and not the
# rootfs: the consumers compile against it at build time, and nothing at runtime
# ever opens a file from it.

NAME    := xproto
VERSION := 7.0.31
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
# Nothing is staged into the overlay or the rootfs; see x11/libX11.
DEPS_ONLY := yes
LIBRARY_PORT := yes

XPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/xproto

include ../../mk/project-port.mk

# Nothing to configure and nothing to compile: the tarball is a directory of
# headers and a Makefile.am.  Upstream's own install would copy *.h into
# $(includedir)/X11, which is done below instead, into build/deps.
# Nothing to configure and nothing to compile: the tarball is a directory of
# headers.  Upstream's own install copies *.h into $(includedir)/X11, which is
# done below instead, into build/deps.
# configure is not optional here.  Xfuncproto.h does not ship as a header: the
# tarball has Xfuncproto.h.in, and upstream registers it as an autoconf config
# header, so it is produced by config.status.  Copying "*.h" without running
# configure stages 24 headers and no Xfuncproto.h, and the very next port then
# stops at:
#
#   ./include/X11/Xdmcp.h:21:10: fatal error: 'X11/Xfuncproto.h' file not found
#
# There is nothing to compile, so BUILD_CMDS stays a no-op.
PROJECT_CFG_CMDS = \
	./configure --prefix=/usr
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging xproto headers\n'
	@mkdir -p "$(XPROTO_PREFIX)/lib/pkgconfig"
	@mkdir -p "$(XPROTO_PREFIX)/include/X11"
	@cp $(SRC_TREE)/*.h "$(XPROTO_PREFIX)/include/X11/"
	@if [ -f "$(SRC_TREE)/X11proto" ]; then \
		$(INSTALL) -m 644 "$(SRC_TREE)/X11proto" "$(XPROTO_PREFIX)/include/X11/"; \
	fi
	@printf '[FreeLinX/ports] staged %s headers\n' "$$(ls "$(XPROTO_PREFIX)/include/X11" | wc -l)"
	@# xproto.pc, because libX11's configure does
	@#   PKG_CHECK_MODULES(X11, [xproto >= 7.0.25 xextproto xtrans xcb >= 1.11.1])
	@# and stops with "Package requirements (xproto >= 7.0.25 ...) unmet" without
	@# one.  Written by hand rather than by configure, since this port has no
	@# configure step: it is a directory of headers.
	@printf 'prefix=%s\nexec_prefix=$${prefix}\nincludedir=$${prefix}/include\nincludex11dir=$${prefix}/include/X11\n\nName: Xproto\nDescription: Xproto headers\nVersion: $(VERSION)\nCflags: -I$${includedir}\n' "$(XPROTO_PREFIX)" > "$(XPROTO_PREFIX)/lib/pkgconfig/xproto.pc"
