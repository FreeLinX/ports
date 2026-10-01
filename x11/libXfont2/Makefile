# FreeLinX/ports - x11/libXfont2 : libXfont2 2.0.8.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libXfont2-2.0.8.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := libXfont2
VERSION := 2.0.8
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXext xtrans fontconfig freetype2 libfontenc
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11 libXext xtrans fontconfig freetype2 libfontenc

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11



# Installed as fontenc.pc, not xfont2.pc: every consumer -- libXfont2's own
# configure and xorg-server -- asks pkg-config for "fontenc".
XORG_PC_NAME := fontenc
XORG_PC_FILE := xfont2

include ../../mk/xorg-lib.mk
