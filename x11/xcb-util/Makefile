# FreeLinX/ports - x11/xcb-util : xcb-util 0.4.1.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/xcb-util-0.4.1.tar.xz
#
# One of the X.Org client libraries, so the recipe comes from mk/xorg-lib.mk and
# only what is specific to this one is written here.
#
# MIT licensed.

NAME    := xcb-util
VERSION := 0.4.1
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libxcb
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

# The archive is -lxcb-util, not -lxcb-util-anything: the port name and the
# -l name agree here, unlike the other four xcb-util packages.
XORG_ARCHIVE := libxcb-util
# xcb-util generates one module per source file, all three naming the same
# archive:
#
#   Libs: -L${libdir} -lxcb-util
#
# xcb-util.pc is the one a consumer names, and xcb-atom.pc, xcb-aux.pc and
# xcb-event.pc are the same library described three more ways.  All four are
# staged: unlike libXaw's xaw6.pc there is no variant here that was not built.
XORG_PC_ALIASES := xcb-atom xcb-aux xcb-event

XORG_LIB_DEPS := libxcb

# libX11's headers are under include/ and under include/X11/, and a consumer
# that includes <X11/Xlib-xcb.h> needs the second.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11

# libxcb's own headers are under include/X11 as well as include/, and these
# libraries include them as <xcb/xcb.h>, so the second directory has to be on
# the path too.
XORG_LIB_CPPFLAGS += -I$(PREFIX_libxcb)/include -I$(PREFIX_libxcb)/include/X11

include ../../mk/xorg-lib.mk
