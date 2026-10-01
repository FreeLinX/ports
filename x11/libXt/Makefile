# FreeLinX/ports - x11/libXt : libXt 1.3.0.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libXt-1.3.0.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := libXt
VERSION := 1.3.0
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXext libXau libXdmcp xtrans libICE libSM
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XORG_LIB_DEPS := libX11 libXext libXau libXdmcp xtrans libICE libSM

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

XORG_PC_NAME := xt

include ../../mk/xorg-lib.mk

# libXt generates src/StringDefs.c with util/makestrs, a noinst_PROGRAMS build
# tool that has to run on this host.  libtool links programs as dynamic PIE by
# default and drops the -static from LDFLAGS on the way through, so makestrs
# comes out wanting /lib/ld-musl-x86_64.so.1, which is not installed here, and
# the build dies at
#
#     ../util/makestrs: cannot execute: required file not found
#
# -all-static is libtool's own flag and it does reach the link.  It goes here,
# at build time, and not in the configure LDFLAGS: configure runs plain
# compiler link tests with no libtool in them, and -all-static is not a
# compiler flag, so configure would report "unrecognized option".
XORG_BUILD_LDFLAGS = -all-static
