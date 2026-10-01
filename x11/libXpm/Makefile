# FreeLinX/ports - x11/libXpm : libXpm 3.5.19.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libXpm-3.5.19.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := libXpm
VERSION := 3.5.19
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXext libICE libSM libxcb libXau libXdmcp
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11 libXext libICE libSM libxcb libXau libXdmcp

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
# sxpm.c includes <X11/StringDefs.h>, which is libXpm's own header and is
# under its own src/ directory, not under include/.
# sxpm is a noinst_PROGRAMS in src/ and uses the Shell widgets, so it links
# libSM and libICE, and configure's own XFONT_LIBS does not name them:
#
#   CCLD     sxpm
#   ld.lld: error: undefined symbol: SmcCloseConnection
XORG_BUILD_LIBS = -lSM -lICE -lX11 -lX11-xcb -lxcb -lXau -lXdmcp

XORG_LIB_CPPFLAGS += -I$(SRC_TREE)/src -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

XORG_PC_NAME := xpm

include ../../mk/xorg-lib.mk
