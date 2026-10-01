# FreeLinX/ports - x11/libXaw : libXaw 1.0.16.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libXaw-1.0.16.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := libXaw
VERSION := 1.0.16
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXext libXt libXmu libXpm libICE libSM
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11 libXext libXt libXmu libXpm libICE libSM

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

XORG_PC_NAME := xaw7

# Two archives, and the one consumers want is the second: libXaw7.pc, the
# X11R6-then-R7 widget set, is what PKG_CHECK_MODULES names.
XORG_ARCHIVE   := libXaw7
XORG_EXTRA_LIBS = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libXaw6.a))

include ../../mk/xorg-lib.mk
