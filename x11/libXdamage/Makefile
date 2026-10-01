# FreeLinX/ports - x11/libXdamage : libXdamage 1.1.6.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libXdamage-1.1.6.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := libXdamage
VERSION := 1.1.6
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXext
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11 libXext libXfixes

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

XORG_PC_NAME := xdamage

include ../../mk/xorg-lib.mk
