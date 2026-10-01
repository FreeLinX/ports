# FreeLinX/ports - x11/xf86-video-fbdev : xf86-video-fbdev 0.5.1.
#
# Real upstream source:
#   https://www.x.org/releases/individual/driver/xf86-video-fbdev-0.5.1.tar.xz
#
# Development repository:
#   https://gitlab.freedesktop.org/xorg/driver/xf86-video-fbdev
#
# The framebuffer video driver, and the reason this port exists in this form: the
# rootfs carried a committed prebuilt fbdev_drv.so stamped "GCC: (GNU) 16.1.1",
# because no port built it.
#
# Why it is not in the xorg-server port.  It is not an oversight there and it is
# not a version skew.  The fbdev DDX was split out of xorg-server into its own
# project, and it is absent from every xorg-server tarball back to 1.20.0:
#
#   xz -dc xorg-server-21.1.16.tar.xz | tar -tf - | grep -c 'hw/xfree86/fbdev/'
#   0
#
# What xorg-server still carries under that name is hw/xfree86/fbdevhw, which is
# the framebuffer hardware shim the driver calls into -- fbdevHWEnterVT,
# fbdevHWDPMSSet and so on, and all of those are among the undefined symbols
# below -- and not a driver.  A driver named fbdev cannot come out of the server
# tarball at all, so this is the source for it.
#
# MIT (X11) licensed.

NAME    := xf86-video-fbdev
VERSION := 0.5.1
CATEGORY:= x11
LICENSE := MIT (X11)

# xorg for the SDK headers and the module ABI, and the two protos its own
# configure.ac asks for by name.  xorg-server.pc carries the rest in
# Requires.private -- xproto, randrproto, renderproto, xextproto, inputproto,
# kbproto, fontsproto, pixman-1, scrnsaverproto, resourceproto, presentproto,
# xineramaproto and pciaccess -- and mk/xorg-lib.mk's --static shim is what
# makes pkg-config follow that list.
DEPENDENCIES:= xorg xproto fontsproto randrproto renderproto xextproto \
	inputproto kbproto pixman
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN := fbdev_drv.so
INSTALL_RELPATH := usr/lib/xorg/modules/drivers/fbdev_drv.so

DEPS = $(FREELINX_BUILD_DIR)/deps

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/src/.libs/fbdev_drv.so
BUILD_BIN   = $(PROJECT_BIN)

PREFIX_xorg = $(DEPS)/xorg

# The driver is dlopen()ed by the server, so unlike every other binary in this
# tree it is a shared object and stays dynamically linked against musl's
# libc.so.  A static module cannot be loaded into a server that resolves the
# Xorg* symbols from its own image, and libtool's own configure asks for this
# with LT_INIT([disable-static]) for the same reason.  No -static here, and none
# in XDRV_LDFLAGS below.
FLX_PROJECT_LIBS :=
# The module is linked without libtool, and libtool is the reason.
#
# libtool assembles the link itself and filters the result: from the user flags and
# from CC it keeps -static-libgcc and drops --target and -resource-dir.  The link
# it builds therefore has no target triple, clang falls back to the host default,
# picks up
#
#   /usr/lib/gcc/x86_64-pc-linux-gnu/16/crtbeginS.o
#
# where compiler-rt's clang_rt.crtbegin-x86_64.o belongs, and that object is
# GCC's and carries
#
#   GCC: (GNU) 16.2.1 20260810
#
# in .comment.  That is the last GNU marker in this port's output, and it is not
# reachable by changing a flag: the flag never makes it onto the command line.
#
# Nothing is given up by linking here.  src/Makefile.am is
#
#   AM_CFLAGS = $(BASE_CFLAGS) $(XORG_CFLAGS)
#   fbdev_drv_la_SOURCES = fbdev.c
#   fbdev_drv_la_LDFLAGS = -module -avoid-version
#
# so there is one translation unit and no -l anything to work out: every symbol
# fbdev.o asks for is either libc's or one the server supplies when it dlopen()s
# the module.  XORG_CFLAGS, which is where -fvisibility=hidden comes from, is
# applied to the compile by automake's own rule, which is the rule below.
XDRV_CFLAGS = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) \
	$(FREELINX_RESOURCE_FLAGS) -O2 -fPIC

# -static-libgcc, with --rtlib=compiler-rt beside it.  clang links a shared object
# with -lgcc_s for the frame registration and on this host that resolves to GCC's:
#
#   ld.lld: error: /usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/16/libgcc_s.so:4:
#            unable to find libgcc_s.so.1
#
# -static-libgcc alone is not enough: it finds that same GCC tree's libgcc.a, and
# the module built from it carries the same GCC: comment.  The two together link
# clang's compiler-rt, and the result needs libc.so and nothing else -- which is
# what the other two modules in the rootfs need, input/evdev_drv.so and
# libfbdevhw.so.
XDRV_LINK = $(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) \
	$(FREELINX_RESOURCE_FLAGS) -fuse-ld=lld --rtlib=compiler-rt \
	-static-libgcc -shared -Wl,-soname,fbdev_drv.so \
	-L$(PREFIX_xorg)/lib

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--with-xorg-module-dir=/usr/lib/xorg/modules \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(XDRV_CFLAGS)" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -fuse-ld=lld --rtlib=compiler-rt"

# Two steps: libtool's compile rule for the object, then the link above.  The
# object is asked for by its libtool name, fbdev.lo, because src/Makefile.am's
# fbdev_drv_la_SOURCES makes it a per-target object and libtool names those
# <target>-<source>.lo.  The real object is the .libs/fbdev.o beside it.
BUILD_CMDS = \
	$(MAKE) -C "$(SRC_TREE)/src" -j"$(FREELINX_JOBS)" fbdev.lo && \
	$(MKDIR) -p "$(SRC_TREE)/src/.libs" && \
	$(XDRV_LINK) -o "$(SRC_TREE)/src/.libs/fbdev_drv.so" \
		"$(SRC_TREE)/src/.libs/fbdev.o"
