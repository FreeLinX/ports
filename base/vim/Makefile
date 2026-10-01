# FreeLinX/ports - base/vim : Vim 9.2.1025 (upstream project port).
#
# Real upstream Vim (github.com/vim/vim tag v9.2.1025) built with the FreeLinX
# clang + LLD + musl toolchain, fully static.  Dependencies:
#   - ncurses (base/ncurses, static, wide-char) for terminfo/tgetent
# Disabled: GUI (X11/GTK), all interpreter bindings (python/lua/ruby/perl/tcl),
# clipboard, netbeans, channel, XSMP, NLS (gettext), gpm, selinux/smack.
# The result is a genuine statically-linked musl Vim with NO GNU runtime
# dependency.  Vim is licensed under the Vim License (charityware) and is NOT
# a GNU component.
#
# Vim needs terminfo entries at runtime; base/ncurses generates
# $(NCURSES_PREFIX)/share/terminfo which is staged into the rootfs at
# /usr/share/terminfo by the install step below.

NAME   := vim
VERSION:= 9.2.1025
CATEGORY:= base
LICENSE:= Vim (charityware)
DEPENDENCIES:= ncurses
BUILD_DEPENDENCIES:= musl, ncurses (base/ncurses, static, wide-char)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= vim
INSTALL_RELPATH:= bin/vim

# Build-time dependency prefix from base/ncurses.
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so this must resolve lazily.
NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/ncurses

# Vim is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(NCURSES_PREFIX)/include
FLX_PROJECT_LDFLAGS  = -L$(NCURSES_PREFIX)/lib

# The FreeLinX toolchain's llvm-ar/llvm-ranlib require GLIBC_2.38 (host has
# 2.36) and cannot run here; archiving is libc-agnostic so host ar/ranlib
# package the musl .o files.  The final Vim link uses ONLY clang+LLD.
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/src/vim
BUILD_BIN   = $(PROJECT_BIN)

# Vim's configure wants a terminal library for tgetent(); give it the static
# wide-char ncurses from base/ncurses.  Everything a static headless editor
# does NOT need is turned off.
# The target triple and the sysroot go in CC, and not only in CFLAGS and LDFLAGS.
# vim compiles some files from shell scripts -- src/osdef.sh builds osdef0.c and
# os_unixx.h from there -- and those scripts call $CC with a CFLAGS of vim's own
# making.  With the flags only in CFLAGS the configure CFLAGS, osdef.sh's compile
# had no --sysroot and picked up the host's headers:
#
#   CC="/.../clang -Iproto -DHAVE_CONFIG_H -I.../ncurses/include"
#   In file included from osdef0.c:7:
#   ./os_unixx.h:38:11: fatal error: 'stropts.h' file not found
#
# and musl's stropts.h -- which the sysroot does have -- is not what it was
# looking for.  In CC they reach every compile in the tree.
VIM_CC = $(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) \
	$(FREELINX_RESOURCE_FLAGS)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--with-tlib=ncursestw \
		--with-features=normal \
		--enable-multibyte \
		--disable-gui --without-x --without-wayland \
		--without-python --without-python3 \
		--without-lua --without-ruby --without-perl --without-tcl \
		--disable-netbeans --disable-channel --disable-xsmp \
		--disable-nls --without-gpm --without-selinux --without-smack \
		--disable-option-checking \
		CC="$(VIM_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -I$(NCURSES_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib" \
		LIBS="-lncursestw" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

# configure detects host cairo/pango in auto/config.mk and ALL_LIBS always
# emits $(CAIRO_LIBS)/$(PANGO_LIBS) (harmless on a link where --as-needed
# applies, but fatal to static lld where the host glibc pango libs simply do
# not exist).  hardcopy-pango/print is disabled anyway; blank them out.
BUILD_CMDS = \
	make -C src -j"$(FREELINX_JOBS)" vim VIMRUNTIMEDIR=/usr/share/vim/vim92 \
		CAIRO_LIBS= PANGO_LIBS=