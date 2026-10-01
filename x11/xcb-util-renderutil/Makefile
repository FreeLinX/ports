# FreeLinX/ports - x11/xcb-util-renderutil : xcb-util-renderutil 0.3.10.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/xcb-util-renderutil-0.3.10.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := xcb-util-renderutil
VERSION := 0.3.10
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libxcb
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

# The archive is named after the module: -lxcb-keysyms, -lxcb-image and so on.
XORG_PC_NAME := xcb-renderutil
XORG_ARCHIVE := libxcb-render-util

XORG_LIB_DEPS := libxcb

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11


# libxcb's own headers are under include/X11 as well as include/, and these
# libraries include them as <xcb/xcb.h> and <xcb/render.h>, so the second
# directory has to be on the path as well.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libxcb)/include -I$(PREFIX_libxcb)/include/X11
include ../../mk/xorg-lib.mk
