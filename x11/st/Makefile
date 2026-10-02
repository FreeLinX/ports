# FreeLinX/ports - x11/st : suckless terminal emulator.
#
# Simple terminal emulator for X.  MIT license.  Plain POSIX make build.
#
# Flags come from pkg-config rather than from a list of prefixes, because the
# X11 headers here come from five different ports and the link set from seven:
# libX11 stages include/X11/{Xlib,Xutil,Xresource,Xregion,Xcms,cursorfont,
# ImUtil,XKBlib,Xlocale,Xlibint,Xlib-xcb}.h and nothing else; xorgproto stages
# the protocol headers (X.h, Xatom.h, Xfuncproto.h, Xmd.h, Xproto.h); kbproto
# stages keysym.h; libXft stages X11/Xft/Xft.h, which itself includes
# X11/extensions/Xrender.h from libXrender; fontconfig brings expat and freetype2;
# and libX11 in turn needs libxcb, libXau and libXdmcp.  Written out by hand:
#
#   x.c:11:10: fatal error: 'X11/Xatom.h' file not found
#
# and then, once the includes were right,
#
#   ld.lld: error: undefined symbol: xcb_get_setup
#   ld.lld: error: undefined symbol: xcb_connect
#
# There is no deps/X11 either.  -I$(FREELINX_BUILD_DIR)/deps/X11/include named a
# directory that does not exist, so the first failure was Xlib.h.

NAME    := st
VERSION := 0.9.2
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libX11, libXft, libXrender, libxcb, xorgproto, kbproto, fontconfig, freetype2
BUILD_DEPENDENCIES:= X11 libs (build/deps), freetype2
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= st
INSTALL_RELPATH:= usr/bin/st

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/st-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/st
BUILD_BIN   = $(PROJECT_BIN)

# --static, because the link is: Requires.private and Libs.private are where
# libXft's libxcb and libXau are, and without --static they are invisible.
ST_PC_MODULES = xft x11 xrender xproto kbproto fontconfig freetype2

# Empty is a hard error, not something to link around.  A module that resolves
# to nothing here means the dependency was not built, and the link that follows
# would name whatever -l happened to be missing rather than say so.
ST_PC_CFLAGS = $(shell $(FLX_PKG_CONFIG) --static --cflags $(ST_PC_MODULES))
ST_PC_LIBS   = $(shell $(FLX_PKG_CONFIG) --static --libs $(ST_PC_MODULES))

ifeq ($(strip $(ST_PC_CFLAGS)$(ST_PC_LIBS)),)
$(error x11/st: pkg-config found none of $(ST_PC_MODULES) in \
  PKG_CONFIG_LIBDIR=$(PKG_CONFIG_LIBDIR); build the dependencies first)
endif

# No quotes on either.  The recipe writes CFLAGS="$(ST_CFLAGS)", and a quoted
# value here expands to CFLAGS=""..."" -- which the shell reads as one word, so
# make is handed --target=x86_64-linux-musl --sysroot=... as an argument and
# parses it as command-line options rather than as an assignment:
#
#   make: unrecognized option '--sysroot=/.../x86_64-linux-musl'
#   make: unrecognized option '--rtlib=compiler-rt'
#   make: invalid option -- 'a'
#   make: invalid option -- 'c'
#   make: This program built for x86_64-pc-linux-gnu
#   make: *** [../../mk/project-port.mk:206: do-build] Error 2
ST_CFLAGS=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 $(ST_PC_CFLAGS)
ST_LDFLAGS=$(FLX_LINK_FLAGS) $(ST_PC_LIBS)

# st's own config.mk also runs `pkg-config --cflags fontconfig` and
# `pkg-config --libs fontconfig` from inside its recipes.  Those are in the
# environment of every recipe here, so they resolve against this tree's modules
# rather than the build host's /usr/lib/pkgconfig.  What that looked like before:
#
#   clang -o st st.o x.o -L/usr/X11R6/lib -lm -lrt -lX11 -lutil -lXft \
#         `pkg-config --libs fontconfig` `pkg-config --libs freetype2` ...
#   ld.lld: error: unable to find library -lbz2
#   ld.lld: error: unable to find library -lpng16
#   ld.lld: error: unable to find library -lz
#   ld.lld: error: unable to find library -lbrotlidec
#   ld.lld: error: unable to find library -lbrotlicommon
#   ld.lld: error: unable to find library -lexpat
#
# Six host libraries named by the host's fontconfig.pc, on a build whose whole
# point is that it is not glibc's.
BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="$(ST_CFLAGS)" \
		LDFLAGS="$(ST_LDFLAGS)"
