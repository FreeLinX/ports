# FreeLinX/ports - x11/xtrans : X11 transport macros 1.6.0 (header-only port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/lib/xtrans-1.6.0.tar.gz
#
# Xtrans is the set of macros the X11 libraries use to talk to the transport
# layer -- socket or otherwise.  Every X client library includes
# <X11/Xtrans/Xtrans.h>, so without this one libX11 stops at
#
#   imTrans.c:59:10: fatal error: 'X11/Xtrans/Xtrans.h' file not found
#
# Two things make this port different from x11/xproto next door:
#
#   * The tarball has no configure and nothing is generated, so there is no
#     configure step.  xproto's Xfuncproto.h comes from a .h.in; nothing here
#     does.
#   * The headers go into include/X11/Xtrans/, NOT include/X11/.  Xtrans is a
#     nested namespace, and libX11 includes them with the X11/Xtrans/ prefix.
#     Copying them flat produces a tree where every header is present and the
#     build still fails to find one.

NAME    := xtrans
VERSION := 1.6.0
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XTRANS_PREFIX = $(FREELINX_BUILD_DIR)/deps/xtrans

include ../../mk/project-port.mk

PROJECT_CFG_CMDS := :
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging xtrans headers\n'
	@mkdir -p "$(XTRANS_PREFIX)/include/X11/Xtrans" "$(XTRANS_PREFIX)/lib/pkgconfig"
	@cp $(SRC_TREE)/*.h "$(XTRANS_PREFIX)/include/X11/Xtrans/"
	@# The .c files too, and this is not optional.  libX11's input-method module
	@# does #include <X11/Xtrans/transport.c>, so a tree with the headers and
	@# none of the sources stops at
	@#   xim_trans.c:30:10: fatal error: 'X11/Xtrans/transport.c' file not found
	@cp $(SRC_TREE)/*.c "$(XTRANS_PREFIX)/include/X11/Xtrans/"
	@sed -e "s|@prefix@|$(XTRANS_PREFIX)|g" -e "s|@exec_prefix@|$${prefix}|g" -e "s|@includedir@|$${prefix}/include|g" -e "s|@PACKAGE_VERSION@|$(VERSION)|g" "$(SRC_TREE)/xtrans.pc.in" > "$(XTRANS_PREFIX)/lib/pkgconfig/xtrans.pc"
	@printf '[FreeLinX/ports] staged %s headers\n' "$$(ls "$(XTRANS_PREFIX)/include/X11/Xtrans" | wc -l)"
