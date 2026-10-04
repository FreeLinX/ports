# FreeLinX/ports - base/vim : Vim 9.2.1025 (upstream project port).
#
# Real upstream Vim (github.com/vim/vim tag v9.2.1025) built with the FreeLinX
# clang + LLD + musl toolchain, fully static.  Dependencies:
#   - libterminfo from NetBSD curses (devel/netbsd-curses, static) for tgetent
# Disabled: GUI (X11/GTK), all interpreter bindings (python/lua/ruby/perl/tcl),
# clipboard, netbeans, channel, XSMP, NLS (gettext), gpm, selinux/smack.
# The result is a genuine statically-linked musl Vim with NO GNU runtime
# dependency.  Vim is licensed under the Vim License (charityware) and is NOT
# a GNU component.
#
# Terminal descriptions come from /usr/share/terminfo.cdb (netbsd-curses);
# xterm, linux, screen, tmux and st are also compiled into libterminfo.

NAME   := vim
VERSION:= 9.2.1025
CATEGORY:= base
LICENSE:= Vim (charityware)
DEPENDENCIES:= netbsd-curses
BUILD_DEPENDENCIES:= musl, netbsd-curses (devel/netbsd-curses)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= vim
INSTALL_RELPATH:= bin/vim

# Build-time dependency prefix from devel/netbsd-curses.
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so this must resolve lazily.
NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/netbsd-curses

# Vim is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(NCURSES_PREFIX)/include
FLX_PROJECT_LDFLAGS  = -L$(NCURSES_PREFIX)/lib

# Archives: llvm-ar/llvm-ranlib from the FreeLinX toolchain (no GNU binutils).
FLX_HOST_AR      ?= $(FREELINX_TOOLCHAIN_BIN)/llvm-ar
FLX_HOST_RANLIB  ?= $(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/src/vim
BUILD_BIN   = $(PROJECT_BIN)

# Vim's configure wants a terminal library for tgetent(); give it NetBSD's
# static libterminfo.  Everything a static headless editor
# does NOT need is turned off.
PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--with-tlib=terminfo \
		--with-features=normal \
		--enable-multibyte \
		--disable-gui --without-x --without-wayland \
		--without-python --without-python3 \
		--without-lua --without-ruby --without-perl --without-tcl \
		--disable-netbeans --disable-channel --disable-xsmp \
		--disable-nls --without-gpm --without-selinux --without-smack \
		--disable-option-checking \
		CC="$(VIM_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(NCURSES_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib" \
		LIBS="-lncursesw" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

# configure detects host cairo/pango in auto/config.mk and ALL_LIBS always
# emits $(CAIRO_LIBS)/$(PANGO_LIBS) (harmless on a link where --as-needed
# applies, but fatal to static lld where the host glibc pango libs simply do
# not exist).  hardcopy-pango/print is disabled anyway; blank them out.
# The target triple and the sysroot go in CC, and not only in CFLAGS and
# LDFLAGS.  vim compiles some files from shell scripts -- src/osdef.sh builds
# osdef0.c and os_unixx.h from there -- and those scripts call $CC with a
# CFLAGS of vim's own making.  With the flags only in CFLAGS the configure
# compile and osdef.sh's had no --sysroot and picked up the host's headers:
#
#   CC="/.../clang -Iproto -DHAVE_CONFIG_H -I.../curses/include"
#   In file included from osdef0.c:7:
#   ./os_unixx.h:38:11: fatal error: 'stropts.h' file not found
#
# and musl's stropts.h -- which the sysroot does have -- is not what it was
# looking for.  In CC they reach every compile in the tree.
VIM_CC = $(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) \
	$(FREELINX_RESOURCE_FLAGS)

BUILD_CMDS = \
	make -C src -j"$(FREELINX_JOBS)" vim VIMRUNTIMEDIR=/usr/share/vim/vim92 \
		CAIRO_LIBS= PANGO_LIBS=