# FreeLinX/ports - x11/lemonbar : lemonbar 1.4 (upstream project port).
#
# Real upstream source:
#   https://github.com/silentz/lemonbar/archive/refs/tags/1.4.tar.gz
# Built statically with FreeLinX clang + LLD + musl.
#
# MIT licensed.  A panel for displaying text on an X root window.
#
# This is upstream lemonbar, not the Xft fork that was here before.  The fork
# exists to render with freetype and pulls in libXft, fontconfig, freetype and
# expat behind it; the plain build needs libxcb and libX11 and nothing else.
#
# That is not only simpler, it is the right scope.  This is the base image, the
# user asked for base only, and a text bar has no reason to drag a font stack in
# to do it.  If an Xft lemonbar is ever wanted, x11/fontconfig and x11/libXft
# are the ports to finish, and this Makefile's -lXft/-lfontconfig/-lexpat go
# back with them.
#
# Produces lemonbar.

NAME    := lemonbar
VERSION := 1.4
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libX11 libxcb
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), X11 client libs (x11/libX11, x11/libxcb)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN := lemonbar
INSTALL_RELPATH := usr/bin/lemonbar

PREFIX_libX11 = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_libxcb = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libXau = $(FREELINX_BUILD_DIR)/deps/libXau
PREFIX_libXdmcp = $(FREELINX_BUILD_DIR)/deps/libXdmcp
PREFIX_libXrender = $(FREELINX_BUILD_DIR)/deps/libXrender
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_xtrans = $(FREELINX_BUILD_DIR)/deps/xtrans

# lemonbar is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/lemonbar
BUILD_BIN   = $(PROJECT_BIN)

# Xorgproto's include directory comes first: it ships newer Xfuncproto.h and
# Xmd.h than the xproto 7.0.31 the xorg archive publishes, and every X11
# header in the stack needs the newer macros.
#
# -lX11 -lxcb is the whole dependency list.  -lxcb-randr/-lxcb-xinerama are
# avoided deliberately: they come from the xcb-util-compat port, which is not
# here, and lemonbar links fine without them -- it just does not report RandR
# and Xinerama geometry.  xtrans is needed because libX11's headers include
# <X11/Xtrans/Xtrans.h>.
PROJECT_CFG_CMDS = :

BUILD_CMDS = \
	$(FREELINX_CC) \
		--target=$(FREELINX_TRIPLE) \
		--sysroot=$(FREELINX_SYSROOT) \
		$(FREELINX_RESOURCE_FLAGS) \
		-O2 -std=c99 -Wall -DVERSION='"$(VERSION)"' -D_GNU_SOURCE \
		-I$(PREFIX_xorgproto)/include \
		-I$(PREFIX_libX11)/include \
		-I$(PREFIX_libxcb)/include -I$(PREFIX_libxcb)/include/X11 \
		-I$(PREFIX_libXau)/include \
		-I$(PREFIX_libXdmcp)/include \
		-I$(PREFIX_libXrender)/include \
		-I$(PREFIX_xtrans)/include \
		-o "$(BUILD_BIN)" \
		$(SRC_TREE)/main.c $(SRC_TREE)/parse.c $(SRC_TREE)/ipc.c \
		$(SRC_TREE)/xcb.c $(SRC_TREE)/xcb_util.c \
		-L$(PREFIX_libX11)/lib -L$(PREFIX_libxcb)/lib \
		-L$(PREFIX_libXau)/lib -L$(PREFIX_libXdmcp)/lib \
		-L$(PREFIX_libXrender)/lib \
		-lX11 -lxcb -lm \
		-static -fuse-ld=lld --rtlib=compiler-rt
