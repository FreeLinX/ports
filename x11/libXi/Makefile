# FreeLinX/ports - x11/libXi : libXi 1.7.10.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libXi-1.7.10.tar.gz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# 1.7.10 rather than 1.8.3, and not for the code.  libXi 1.8.x asks
# PKG_CHECK_MODULES for "inputproto >= 2.3.99.1", which is the 2.4 development
# series: inputproto has never been released past 2.3.2, so no inputproto
# tarball that exists can satisfy it.  1.7.10 asks for
# "inputproto >= 2.2.99.1", which 2.3.2 satisfies.
#
# This is not a library the X server needs.  It is here because libXtst's
# XTest.h does #include <X11/extensions/XInput.h>, and XInput.h is libXi's, not
# inputproto's -- inputproto ships XI.h.  Without libXi, libXtst stops at
# "fatal error: 'X11/extensions/XInput.h' file not found" and everything that
# links libXtst, xorg-server included, stops with it.
#
# MIT licensed.

NAME    := libXi
VERSION := 1.7.10
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXext libXfixes
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11 libXext libXfixes

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

XORG_PC_NAME := xi

include ../../mk/xorg-lib.mk
