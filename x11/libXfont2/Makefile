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

# libXfont2's own build links -lXfont2 for a helper of its own, so the
# archive has to be reachable by name while it is being built.
XORG_EXTRA_LDFLAGS = -L$(SRC_TREE)/.libs

# libXfont2's own programs link -lXfont2 and then need the libraries it
# was built against, in that order: the archive first, its dependencies
# after it, or lld reports FontEncFromXLFD undefined.
XORG_BUILD_LIBS = -lfontenc -lfreetype -lz -lX11 -lXext

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11
# Two module names for two different libraries, and that is how it should be:
# libXfont2's configure asks pkg-config for "fontenc" and xorg-server asks for
# "xfont2 >= 2.0.0".  They are not the same library.  xfont2 is this one;
# fontenc is libfontenc, which is where FontEncFromXLFD and FontEncFind live, and
# x11/libfontenc stages a fontenc.pc of its own.
#
# An alias here that copied xfont2.pc to fontenc.pc was worse than nothing: the
# copy carries Requires.private: fontenc, so pkg-config sees fontenc require
# itself, drops the cycle, and xorg-server's link loses the -lfontenc that was
# the only thing it was missing --
#
#   undefined reference to `FontEncFromXLFD'
#
# with nothing in the log to say a pkg-config cycle ate it.
XORG_PC_NAME := xfont2
XORG_PC_NAME    := xfont2

include ../../mk/xorg-lib.mk
