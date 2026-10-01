# FreeLinX/ports - x11/libfontenc : libfontenc 1.1.9.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libfontenc-1.1.9.tar.xz
#
# One of the X.Org autotools libraries, so the recipe comes from mk/xorg-lib.mk.
#
# It is here because libXfont2's configure asks pkg-config for
#
#     xproto xtrans fontsproto >= 2.1.3 fontenc
#
# and fontenc is this library's module name, not a header package.  It installs
# include/X11/fonts/fontenc.h, which is what libXfont2's src/FreeType/ftenc.c
# includes, so both the requirement and the header are satisfied by building it
# rather than by any header package.  It is also a real link dependency:
# libXfont2.a calls FontEncName and FontEncDirectory.
#
# MIT licensed.

NAME    := libfontenc
VERSION := 1.1.9
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := zlib

# No X11 dependency at all: this *is* the fontenc library.  Only zlib's headers,
# and the headers install to include/X11/fonts/ rather than include/X11/.
XORG_LIB_CPPFLAGS += -I$(FREELINX_BUILD_DIR)/deps/zlib/include

XORG_PC_NAME := fontenc

include ../../mk/xorg-lib.mk
