# FreeLinX/ports - x11/xorg : Xorg server 21.1.16.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/xserver/xorg-server-21.1.16.tar.xz
#
# The X server, and the reason this port exists: the rootfs carried a committed
# prebuilt Xorg stamped "GCC: (GNU) 16.1.1" together with a matching
# usr/lib/xorg/modules/drivers/fbdev_drv.so, because no port built them.
#
# MIT (X11) licensed.
#
# What is switched off, and why.  FreeLinX runs the server on fbdev and on
# plain VTs, not on DRM/KMS with glamor, systemd logind, D-Bus device discovery
# or SELinux, and every one of those would need another port:
#
#   --disable-glamor        no libepoxy, and no GL
#   --disable-libdrm        no libdrm; see the DRM= note below
#   --disable-dri           no DRI 1.0
#   --disable-dri2 --disable-dri3
#   --disable-glx
#   --disable-udev          no libudev; the server falls back to /dev nodes
#   --disable-dbus          no D-Bus device discovery
#   --disable-systemd-logind
#   --disable-selinux
#   --disable-libunwind
#   --disable-xephyr        no nested server; that is what wants xcb-renderutil
#   --disable-xvfb --disable-xv
#   --disable-panfrost --disable-vmwgfx --disable-nouveau
#
# DRM, DRI, DRI2, DRI3, VGAHW, PCI, DGA, XSELINUX, CONFIG_HAL, CONFIG_UDEV and
# CONFIG_UDEV_KMS are also named as variables on the configure line, and that is
# not belt to the switches -- it is the only thing that works.  configure.ac
# decides them in a `case $host_os in` and this toolchain's triple is
# x86_64-linux-musl, so host_os is linux-musl, which matches neither the gnu* nor
# the linux* case: every one of them keeps its `auto' default, the auto resolves
# to yes, and the build then stops at
#
#   configure: error: Package requirements (libdrm >= 2.4.89) were not met
#
# with no hint that --disable-libdrm was ignored.  A variable named on the
# command line is immutable in an autoconf script, so this is the way to say no.
#
# What is switched on, and why:
#
#   --with-sha1=libcrypto   musl has no SHA-1 and the server refuses to
#                           configure without one; see devel/openssl
#
# The XKB data is already in the rootfs at /usr/share/X11/xkb, so
# xkeyboard-config is a runtime data dependency and not a build one.
#
# The link is static against musl, so the server needs every library it
# references at build time: pixman, libXfont2, libfontenc, libXau, libXdmcp,
# xcb and its extensions, libX11, libXext, libXrender, libXfixes, libXdamage,
# libXrandr, libXinerama, libXxf86vm, libXtst, libXt, libXmu, libXaw, libXpm,
# libxkbfile, libpciaccess, libxcvt and fontconfig/freetype through libXfont2.

NAME    := xorg
VERSION := 21.1.16
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= \
	pixman libXfont2 libfontenc fontconfig freetype2 zlib openssl \
	libXau libXdmcp libxcb libX11 libXext libXrender libXfixes libXdamage \
	libXrandr libXinerama libXxf86vm libXtst libXt libXmu libXaw libXpm \
	libxkbfile libpciaccess libxcvt xorgproto xtrans xproto xextproto \
	kbproto inputproto renderproto
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN := Xorg
INSTALL_RELPATH := usr/bin/Xorg

DEPS = $(FREELINX_BUILD_DIR)/deps

include ../../mk/project-port.mk

# hw/xfree86/Xorg, not $(SRC_TREE)/Xorg: the binary is built by the hw/xfree86
# subdirectory and the top level has no copy of it.
PROJECT_BIN = $(SRC_TREE)/hw/xfree86/Xorg
BUILD_BIN   = $(PROJECT_BIN)

PREFIX_xorgproto   = $(DEPS)/xorgproto
PREFIX_xtrans      = $(DEPS)/xtrans
PREFIX_xproto      = $(DEPS)/xproto
PREFIX_xextproto   = $(DEPS)/xextproto
PREFIX_kbproto     = $(DEPS)/kbproto
PREFIX_inputproto  = $(DEPS)/inputproto
PREFIX_renderproto = $(DEPS)/renderproto
PREFIX_pixman      = $(DEPS)/pixman
PREFIX_libXfont2   = $(DEPS)/libXfont2
PREFIX_libfontenc  = $(DEPS)/libfontenc
PREFIX_fontconfig  = $(DEPS)/fontconfig
PREFIX_freetype2   = $(DEPS)/freetype2
PREFIX_libX11      = $(DEPS)/libX11
PREFIX_libxcb      = $(DEPS)/libxcb
PREFIX_libXau      = $(DEPS)/libXau
PREFIX_libXdmcp    = $(DEPS)/libXdmcp
PREFIX_libXext     = $(DEPS)/libXext
PREFIX_libXrender  = $(DEPS)/libXrender
PREFIX_libXfixes   = $(DEPS)/libXfixes
PREFIX_libXdamage  = $(DEPS)/libXdamage
PREFIX_libXrandr   = $(DEPS)/libXrandr
PREFIX_libXinerama = $(DEPS)/libXinerama
PREFIX_libXxf86vm  = $(DEPS)/libXxf86vm
PREFIX_libXtst     = $(DEPS)/libXtst
PREFIX_libXt       = $(DEPS)/libXt
PREFIX_libXmu      = $(DEPS)/libXmu
PREFIX_libXaw      = $(DEPS)/libXaw
PREFIX_libXpm      = $(DEPS)/libXpm
PREFIX_libXkbfile  = $(DEPS)/libXkbfile
PREFIX_libpciaccess = $(DEPS)/libpciaccess
PREFIX_libxcvt     = $(DEPS)/libxcvt
PREFIX_zlib        = $(DEPS)/zlib
PREFIX_openssl     = $(DEPS)/openssl
PREFIX_xcb-util     = $(DEPS)/xcb-util
PREFIX_xcb-util-keysyms = $(DEPS)/xcb-util-keysyms
PREFIX_xcb-util-image   = $(DEPS)/xcb-util-image
PREFIX_xcb-util-wm      = $(DEPS)/xcb-util-wm
PREFIX_xcb-util-renderutil = $(DEPS)/xcb-util-renderutil
PREFIX_xcb = $(FREELINX_BUILD_DIR)/deps/xcb

# xorgproto first, and the reason is the same one mk/xorg-lib.mk gives: its
# Xfuncproto.h and Xmd.h are newer than the xproto 7.0.31 the x.org archive
# still publishes, and the server's headers want the newer macros.  xproto's
# XKBproto.h puts _X_NONSTRING after the array bound, which clang will not
# accept in any position -- x11/xorgproto's patch drops the attribute, and
# without that patch this is where the build stops:
#
#   XKBproto.h:679:33: error: expected ';' at end of declaration list
XORG_CPPFLAGS = \
	-I$(PREFIX_xorgproto)/include \
	-I$(PREFIX_xproto)/include \
	-I$(PREFIX_xextproto)/include \
	-I$(PREFIX_kbproto)/include \
	-I$(PREFIX_inputproto)/include \
	-I$(PREFIX_renderproto)/include \
	-I$(PREFIX_xtrans)/include \
	-I$(SRC_TREE)/include \
	-I$(DEPS)/pixman/include/pixman-1 \
	-I$(PREFIX_libX11)/include -I$(PREFIX_libX11)/include/X11 \
	-I$(PREFIX_libxcb)/include \
	-I$(PREFIX_libXau)/include -I$(PREFIX_libXdmcp)/include \
	-I$(PREFIX_libXfont2)/include -I$(PREFIX_libfontenc)/include \
	-I$(PREFIX_libXkbfile)/include \
	-I$(PREFIX_fontconfig)/include -I$(PREFIX_freetype2)/include \
	-I$(PREFIX_libXmu)/include -I$(PREFIX_libXaw)/include \
	-I$(PREFIX_libXpm)/include -I$(PREFIX_libXt)/include \
	-I$(PREFIX_libXext)/include -I$(PREFIX_libXrender)/include \
	-I$(PREFIX_libpciaccess)/include -I$(PREFIX_libxcvt)/include \
	-I$(PREFIX_zlib)/include -I$(PREFIX_openssl)/include

XORG_LDFLAGS = \
	-L$(DEPS)/pixman/lib \
	-L$(PREFIX_libXfont2)/lib -L$(PREFIX_libfontenc)/lib \
	-L$(PREFIX_libX11)/lib -L$(PREFIX_libxcb)/lib \
	-L$(PREFIX_libXau)/lib -L$(PREFIX_libXdmcp)/lib \
	-L$(PREFIX_libXext)/lib -L$(PREFIX_libXrender)/lib \
	-L$(PREFIX_libXfixes)/lib -L$(PREFIX_libXdamage)/lib \
	-L$(PREFIX_libXrandr)/lib -L$(PREFIX_libXinerama)/lib \
	-L$(PREFIX_libXxf86vm)/lib -L$(PREFIX_libXtst)/lib \
	-L$(PREFIX_libXt)/lib -L$(PREFIX_libXmu)/lib -L$(PREFIX_libXaw)/lib \
	-L$(PREFIX_libXpm)/lib -L$(PREFIX_libXkbfile)/lib \
	-L$(PREFIX_libpciaccess)/lib -L$(PREFIX_libxcvt)/lib \
	-L$(PREFIX_fontconfig)/lib -L$(PREFIX_freetype2)/lib \
	-L$(PREFIX_zlib)/lib -L$(PREFIX_openssl)/lib \
	-L$(PREFIX_xcb-util)/lib -L$(PREFIX_xcb-util-keysyms)/lib \
	-L$(PREFIX_xcb-util-image)/lib -L$(PREFIX_xcb-util-wm)/lib \
	-L$(PREFIX_xcb-util-renderutil)/lib

# One order for all of them, and lld will not repeat an archive for a second
# pass, so the whole set goes in a group rather than being guessed at.  Plain
# -l flags, not -Wl,--start-group: libtool reorders LDFLAGS and rewrites
# -Wl,--start-group into a bare --start-group that clang rejects.
XORG_LIBS = \
	-lXfont2 -lfontenc -lXaw7 -lXpm -lXtst -lXrandr -lXinerama \
	-lXdamage -lXfixes -lXxf86vm -lXrender -lXext -lXt -lXau -lXdmcp \
	-lX11 -lX11-xcb -lpciaccess -lxcvt -lxkbfile -lpixman-1 \
	-lxcb-shape -lxcb-randr -lxcb-render -lxcb-xfixes -lxcb-shm \
	-lxcb-keysyms -lxcb-image -lxcb-icccm -lxcb-util -lxcb \
	-lfontconfig -lfreetype -lz -lcrypto -lpthread -lm -ldl

# The prefix configure runs with.  stage-sdk needs it too: xorg-server.pc is
# generated with it baked in, and the module file the fbdev driver port reads
# has to name the staged dependency tree instead, because mk's
# scripts/check-deps-pkgconfig.sh rejects a staged .pc whose prefix is outside
# the tree.
XORG_PREFIX := /usr

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=$(XORG_PREFIX) \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--disable-glamor \
		--disable-libdrm \
		--disable-dri \
		--disable-dri2 \
		--disable-dri3 \
		--disable-glx \
		--disable-udev \
		--disable-dbus \
		--disable-systemd-logind \
		--disable-selinux \
		--disable-libunwind \
		--disable-xephyr \
		--disable-xvfb \
		--disable-xv \
		--disable-xdmcp-wrap \
		--disable-panfrost \
		--disable-vmwgfx \
		--disable-nouveau \
		--disable-linux-modeset \
		--without-x \
		--with-sha1=libcrypto \
		--with-xkb-base-directory=/usr/share/X11/xkb \
		--with-xkb-output-directory=/var/lib/xkb \
		--with-fpc-glx=no \
		--with-systemdsystemunitdir=/usr/lib/systemd/system \
		DRM=no \
		DRI=no \
		DRI2=no \
		DRI3=no \
		VGAHW=no \
		INT10MODULE=no \
		PCI=no \
		DGA=no \
		XSELINUX=no \
		SYMBOL_VISIBILITY=no \
		CONFIG_HAL=no \
		CONFIG_UDEV=no \
		CONFIG_UDEV_KMS=no \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fno-strict-aliasing $(XORG_CPPFLAGS)" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt $(XORG_LDFLAGS)"

# "all" does not build Xorg: the top level walks SUBDIRS, which ends in hw, and
# hw builds Xnest, which does not link in a static tree.  So the library
# subdirectories are walked here and the one artifact the rootfs wants is named
# as a target.
#
# The list is the configured SUBDIRS with doc, man, test and hw removed:
#
#   doc man include dix fb mi Xext miext os randr render Xi xkb  dbe record
#   xfixes damageext composite  present  exa  config hw test
#
# hw/vfb builds Xnest and test/ builds the server's test programs.  Both link
# the server's own xkb/libxkb.a and libX11.a together.  The two define the same
# XKB client symbols, which is fine when libX11 is a shared object -- its
# symbols are in the .so and the static one wins -- but not when it is
# libX11.a:
#
#   ld.lld: error: duplicate symbol: XkbFreeGeomOverlayKeys
#   >>>            XKBGAlloc.o:(XkbFreeGeomOverlayKeys) in archive ../../xkb/.libs/libxkb.a
#   >>> defined at XKBGAlloc.c
#   >>>            XKBGAlloc.o:(.text+0x5c0) in archive .../libX11.a
#
# Xnest exists only to host Xephyr and Xephyr is switched off; the test
# programs are not part of the product.  doc and man are docbook and neither is
# in the rootfs.
XORG_SUBDIRS = include dix fb mi Xext miext os randr render Xi xkb dbe \
	record xfixes damageext composite present exa config

# Two make invocations, not one, because the two artifacts need different link
# flags.  Xorg is a program and libtool links programs as dynamic PIE by
# default, dropping the -static from LDFLAGS on the way through -- the same trap
# x11/libXt's util/makestrs fell into.  Without the separate -all-static pass
# Xorg comes out wanting /lib/ld-musl-x86_64.so.1 while every other binary in
# the rootfs is static.  -all-static is a libtool flag and belongs at build time
# only: configure runs plain compiler link tests that have never heard of it.
#
# The binary is removed between the two passes.  hw/xfree86's own "all" links
# Xorg, so by the time the second pass asks for that target it is up to date,
# make reports nothing to be done, and the server stays a dynamic PIE with an
# interpreter.  Without -all-static and without the rm:
#
#   ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked,
#   interpreter /lib/ld-musl-x86_64.so.1
XORG_LINK = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt $(XORG_LDFLAGS)

# hw/xfree86 is built whole before its Xorg target is asked for, because a target
# build does not descend into SUBDIRS and Xorg depends on loader/libloader.la,
# which lives in hw/xfree86/loader.  Asking for the target alone stops at
#
#   make[1]: *** No rule to make target 'loader/libloader.la', needed by 'Xorg'.
#
# Every comment here is above the recipe rather than inside it.  A comment line
# between two backslash-continued recipe lines ends the recipe, and the line
# after it is read as a recipe with no target at all:
#
#   Makefile:295: *** recipe commences before first target.  Stop.
#
# The same thing happens inside a shell block, where the comment swallows the
# continuation and the else after it disappears:
#
#   /bin/sh: -c: line 5: syntax error: unexpected end of file from `if'
BUILD_CMDS = \
	for d in $(XORG_SUBDIRS); do \
		$(MAKE) -C "$(SRC_TREE)/$$d" -j"$(FREELINX_JOBS)" LDFLAGS="$(XORG_LINK)" || exit 1; \
	done && \
	$(MAKE) -C "$(SRC_TREE)/hw/xfree86" -j"$(FREELINX_JOBS)" LDFLAGS="$(XORG_LINK)" && \
	rm -f "$(SRC_TREE)/hw/xfree86/Xorg" && \
	$(MAKE) -C "$(SRC_TREE)/hw/xfree86" -j"$(FREELINX_JOBS)" Xorg LDFLAGS="$(XORG_LINK) -all-static"

# ---------------------------------------------------------------------------
# The driver SDK: the headers and the .pc that x11/xf86-video-fbdev needs.
#
# xorg-server's fbdev DDX was split out of the server into its own project in
# 2015, so the rootfs's second missing binary -- usr/lib/xorg/modules/drivers/
# fbdev_drv.so -- is not something this tree can build out of the server
# tarball.  It is not in 21.1.16, nor in 1.20.14, nor in 21.1.0; what is left
# under hw/xfree86/fbdevhw is the framebuffer hardware shim, not a driver.  The
# driver lives at https://gitlab.freedesktop.org/xorg/driver/xf86-video-fbdev
# and asks pkg-config for:
#
#   PKG_CHECK_MODULES(XORG, [xorg-server >= 1.18 xproto fontsproto ...])
#
# xorg-server.pc is built from xorg-server.pc.in with sdkdir and moduledir
# substituted, so it is generated here and its paths are moved into the
# dependency tree.  config.status takes no VAR=VALUE overrides -- its usage is
# "[OPTION]... [TAG]..." and nothing else -- so the rewrite is a sed over the
# six path lines, with $(XORG_PREFIX) naming what configure was given.
#
# The headers come from the tree's own install-sdkHEADERS targets rather than
# from a list here: sdk_HEADERS is declared in a dozen subdirectories and
# include/Makefile.am alone holds 60 entries, and any list written here would be
# wrong the moment one of them changed.
#
# Upstream's own install-headers target cannot be reused.  It is
#
#     find . -name Makefile | while read m; do
#             if grep -q install-sdkHEADERS $$m; then
#                     (cd `dirname "$$m"` && make install-sdkHEADERS)
#             fi
#     done
#
# and the top-level Makefile matches that grep -- the target's own recipe is
# inside it -- so the loop ends with
#
#     make[2]: *** No rule to make target 'install-sdkHEADERS'.  Stop.
#
# The grep below anchors on '^install-sdkHEADERS:', which is a target and not a
# mention, and sdkdir is passed to each sub-make because sdkdir is a make
# variable and an argument to the outer make does not reach the inner ones:
#
#     /usr/bin/mkdir -p '/usr/include/xorg'
#     mkdir: cannot create directory '/usr/include/xorg': Permission denied
#
# hw/xfree86/common is copied too, and that is where the fbdev driver stops
# without it.  common/Makefile.am declares most of its headers as sdk_HEADERS, so
# install-sdkHEADERS already put them in, but three are behind
#
#     if XV
#     XVSDKINCS = xf86xv.h xf86xvmc.h xf86xvpriv.h
#
# and this port passes --disable-xv.  x11/xf86-video-fbdev's single source file
# says
#
#     #include "xf86xv.h"
#
# and uses nothing from it, so the compile stops at
#
#     fbdev.c:28:10: fatal error: 'xf86xv.h' file not found
#
# The whole directory is copied rather than those three by name: the point is the
# interface a DDX is allowed to include, and the set of headers that interface
# is made of is decided upstream, in a file this port does not read.  The
# sdk_HEADERS in that directory are copied a second time from the same source
# tree, identically.
#
# xorg-server.h is copied by hand.  It is nodist_sdk_HEADERS rather than
# sdk_HEADERS in include/Makefile.am, and the generated target is
#
#     install-sdkHEADERS: $(sdk_HEADERS)
#
# so xorg-server.h is not in it.  The driver needs it: the driver's configure.ac
# says AH_TOP([#include "xorg-server.h"]), which makes config.h include it, and
# it is generated from xorg-server.h.in by the include/ build.
XORG_SDK = $(DEPS)/xorg

.PHONY: stage-sdk
install: stage-sdk

stage-sdk:
	@printf '[FreeLinX/ports] staging the %s driver SDK -> %s\n' "$(NAME)" "$(XORG_SDK)"
	$(MKDIR) -p "$(XORG_SDK)/lib/pkgconfig"
	cd "$(SRC_TREE)" && find . -name Makefile | while read -r m; do \
		grep -q '^install-sdkHEADERS:' "$$m" || continue; \
		$(MAKE) -C "$$(dirname "$$m")" install-sdkHEADERS sdkdir="$(XORG_SDK)/include" || exit 1; \
	done
	$(MAKE) -C "$(SRC_TREE)" xorg-server.pc
	$(INSTALL) -m 644 "$(SRC_TREE)/include/xorg-server.h" "$(XORG_SDK)/include/"
	$(INSTALL) -m 644 "$(SRC_TREE)"/hw/xfree86/common/*.h "$(XORG_SDK)/include/"
	sed -e 's|^prefix=.*|prefix=$(XORG_SDK)|' \
	    -e 's|^exec_prefix=.*|exec_prefix=$${prefix}|' \
	    -e 's|^libdir=.*|libdir=$${prefix}/lib|' \
	    -e 's|^includedir=.*|includedir=$${prefix}/include|' \
	    -e 's|^datarootdir=.*|datarootdir=$${prefix}/share|' \
	    -e 's|^moduledir=.*|moduledir=$${prefix}/lib/xorg/modules|' \
	    -e 's|^sdkdir=.*|sdkdir=$${prefix}/include|' \
	    -e 's|^sysconfigdir=.*|sysconfigdir=$${prefix}/etc|' \
	    "$(SRC_TREE)/xorg-server.pc" > "$(XORG_SDK)/lib/pkgconfig/xorg-server.pc"
	@if [ -z "$$(find "$(XORG_SDK)/include" -name 'xorg-server.h' -print -quit)" ]; then \
		printf '[FreeLinX/ports][error] %s: the SDK has no xorg-server.h; a driver cannot be built against it\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@printf '[FreeLinX/ports]   %s SDK headers, xorg-server.pc\n' "$$(find "$(XORG_SDK)/include" -name '*.h' | wc -l | tr -d ' ')"
