# FreeLinX/ports - x11/xkbcomp : XKB keymap compiler 1.4.7.
#
# Real upstream source:
#   https://github.com/freedesktop/xorg-x11-xkb-utils (xkbcomp moved here from
#   libX11 and X11R6 in 2008; 7.7+9 is the current release)
#   https://codeload.github.com/freedesktop/xorg-x11-xkb-utils/tar.gz/refs/tags/X11R6-7.7+9
# Built statically with FreeLinX clang + LLD + musl.
#
# Produces xkbcomp, which compiles the XML in /usr/share/X11/xkb into a keymap
# the X server loads.  That data is already in the rootfs, so this port builds
# the compiler and nothing else.
#
# Depends on libX11 and libXkbfile, both built here; xorgproto satisfies the
# "xproto >= 7.0.17" that upstream's PKG_CHECK_MODULES asks for.
#
# MIT licensed.

NAME    := xkbcomp
VERSION := 1.4.7
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libxcb libXau libXdmcp libXkbfile xorgproto
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN := xkbcomp
INSTALL_RELPATH := usr/bin/xkbcomp

PREFIX_libX11     = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_libxcb     = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libXau     = $(FREELINX_BUILD_DIR)/deps/libXau
PREFIX_libXdmcp   = $(FREELINX_BUILD_DIR)/deps/libXdmcp
PREFIX_libXkbfile = $(FREELINX_BUILD_DIR)/deps/libXkbfile
PREFIX_xorgproto  = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_xtrans     = $(FREELINX_BUILD_DIR)/deps/xtrans

# xkbcomp is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

# The archive is the x11-xkb-utils repository; xkbcomp is one directory inside
# it, and the tarball's top directory is x11-xkb-utils.git.  SRC_TREE therefore
# has two components, neither of which the framework's default would produce.
SRC_TREE = $(SRC_DIR)/x11-xkb-utils.git/xkbcomp

PROJECT_BIN = $(SRC_TREE)/xkbcomp
BUILD_BIN   = $(PROJECT_BIN)

# The pc files xkbcomp's configure asks pkg-config for: x11.pc from libX11,
# xkbfile.pc from libXkbfile, and xproto.pc from xorgproto.  All three are in
# build/deps because all three ports are dependency-only.
# One line, no continuations.  A backslash-newline inside a variable
# assignment becomes a space, so a wrapped colon-separated list reaches
# pkg-config as "dir1: dir2:" and every element after the first names a
# directory that does not exist.


PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		PKG_CONFIG="/usr/bin/pkg-config" \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -I$(PREFIX_xorgproto)/include -I$(PREFIX_libX11)/include -I$(PREFIX_libXkbfile)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_libX11)/lib -L$(PREFIX_libxcb)/lib -L$(PREFIX_libXau)/lib -L$(PREFIX_libXdmcp)/lib -L$(PREFIX_libXkbfile)/lib"

# libX11 needs libxcb behind it: x11.pc says only -lX11, and libX11.a calls
# xcb_disconnect, xcb_get_setup and xcb_get_maximum_request_length.
BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" LIBS="-lX11 -lX11-xcb -lxcb -lXau -lXdmcp -lxkbfile"
