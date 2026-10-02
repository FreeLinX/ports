# FreeLinX/ports - x11/lemonbar : lemonbar 1.4 (upstream project port).
#
# Real upstream source:
#   https://github.com/silentz/lemonbar-xft/archive/refs/heads/master.tar.gz
# Built statically with FreeLinX clang + LLD + musl.
#
# MIT licensed.  A panel for displaying text on an X root window.
#
# Source note: the upstream repo is silentz/lemonbar-xft, and silentz/lemonbar
# redirects to it -- there is no plain build to fetch.  So this is the Xft fork,
# which is what the rootfs already had anyway; what changed is that every
# library it needs is now built from source here:
#
#   libX11  libxcb  libXrender  libXft  fontconfig  freetype2
#
# Before this, the rootfs's lemonbar was a committed prebuilt binary stamped
# "GCC: (GNU) 16.1.1", kept there because x11/lemonbar pointed at
# ../desktop-test/ports/build/deps/x11 -- a repo that has been deleted.
#
# Produces lemonbar.

NAME    := lemonbar
VERSION := 1.4
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libX11 libxcb libXft libXrender fontconfig freetype2
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN := lemonbar
INSTALL_RELPATH := usr/bin/lemonbar

PREFIX_libX11    = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_libxcb    = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libXau    = $(FREELINX_BUILD_DIR)/deps/libXau
PREFIX_libXdmcp  = $(FREELINX_BUILD_DIR)/deps/libXdmcp
PREFIX_libXrender = $(FREELINX_BUILD_DIR)/deps/libXrender
PREFIX_libXft    = $(FREELINX_BUILD_DIR)/deps/libXft
PREFIX_fontconfig = $(FREELINX_BUILD_DIR)/deps/fontconfig
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_xtrans    = $(FREELINX_BUILD_DIR)/deps/xtrans
# build/deps/freetype2, not $(FREELINX_STAGING_ROOT)/usr: the staging tree is
# where a port leaves what it installs into the rootfs, and a -I or -L into it
# reaches the build host's /usr too.
PREFIX_freetype2 = $(FREELINX_BUILD_DIR)/deps/freetype2
PREFIX_expat = $(FREELINX_BUILD_DIR)/deps/expat
PREFIX_zlib = $(FREELINX_BUILD_DIR)/deps/zlib

# lemonbar is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/lemonbar
BUILD_BIN   = $(PROJECT_BIN)

# Upstream's own Makefile is a few lines, so the build runs its flags here
# instead -- which also means the -I/-L list is one place rather than split
# across a configure invocation that would have to be handed the same list.
#
# xorgproto first: its Xfuncproto.h and Xmd.h are newer than the xproto 7.0.31
# the xorg archive publishes, and the X11 headers need the newer macros.
#
# lemonbar.c includes <xcb/randr.h> and <xcb/xinerama.h> and calls into both,
# so -lxcb-randr and -lxcb-xinerama are not optional.  Those archives are
# separate from libxcb.a: libxcb builds each enabled extension into its own
# libxcb-<ext>.a, and x11/libxcb stages them.
BUILD_CMDS = \
	$(FREELINX_CC) \
		--target=$(FREELINX_TRIPLE) \
		--sysroot=$(FREELINX_SYSROOT) \
		$(FREELINX_RESOURCE_FLAGS) \
		-O2 -std=c99 -Wall -DVERSION='"$(VERSION)"' -D_GNU_SOURCE \
		-I$(PREFIX_xorgproto)/include \
		-I$(PREFIX_libX11)/include \
		-I$(PREFIX_libxcb)/include -I$(PREFIX_libxcb)/include/X11 \
		-I$(PREFIX_libXau)/include -I$(PREFIX_libXdmcp)/include \
		-I$(PREFIX_libXrender)/include -I$(PREFIX_libXft)/include \
		-I$(PREFIX_fontconfig)/include -I$(PREFIX_freetype2)/include \
		-I$(PREFIX_xtrans)/include \
		-o "$(BUILD_BIN)" \
		$(SRC_TREE)/lemonbar.c $(SRC_TREE)/utils.c \
		-L$(PREFIX_libX11)/lib -L$(PREFIX_libxcb)/lib \
		-L$(PREFIX_libXau)/lib -L$(PREFIX_libXdmcp)/lib \
		-L$(PREFIX_libXrender)/lib -L$(PREFIX_libXft)/lib \
		-L$(PREFIX_fontconfig)/lib -L$(PREFIX_freetype2)/lib \
		-L$(PREFIX_expat)/lib -L$(PREFIX_zlib)/lib \
		-Wl,--start-group \
		-lXft -lXrender -lfontconfig -lfreetype -lX11 -lX11-xcb -lXau \
		-lxcb -lxcb-randr -lxcb-xinerama -lexpat -lz -lm \
		-Wl,--end-group \
		-static -fuse-ld=lld --rtlib=compiler-rt
