# FreeLinX/ports - x11/libpciaccess : libpciaccess 0.17.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libpciaccess-0.17.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := libpciaccess
VERSION := 0.17
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
# musl guards pread64() behind _LARGEFILE64_SOURCE, and libpciaccess's
# linux_sysfs.c calls it.  glibc declares it unconditionally, which is why no
# other port needed this.
XORG_LIB_CPPFLAGS += -D_LARGEFILE64_SOURCE -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

XORG_PC_NAME := pciaccess

include ../../mk/xorg-lib.mk
