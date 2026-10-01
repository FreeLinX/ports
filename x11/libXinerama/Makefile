# FreeLinX/ports - x11/libXinerama : libXinerama 1.1.5.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libXinerama-1.1.5.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := libXinerama
VERSION := 1.1.5
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXext
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11 libXext

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

XORG_PC_NAME := xinerama

include ../../mk/xorg-lib.mk
