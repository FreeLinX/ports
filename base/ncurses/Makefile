# FreeLinX/ports - base/ncurses : ncurses 6.5 (build dependency, upstream port).
#
# Real upstream ncurses 6.5 built with the FreeLinX clang + LLD + musl
# toolchain, statically (libncursesw.a / libdecompose...).  It is a build-time
# dependency for base/vim (which links --with-tlib=ncursesw) and is installed
# only into the build-time dependency prefix (build/deps/ncurses); it is
# intentionally NOT staged into the rootfs as a library, since it is linked
# statically into its consumers.
#
# ncurses is NOT a GNU component: it is the independent terminfo/curses
# library (X11/MIT-style license), kept here as a build tool for the static
# Vim port.  No GNU runtime dependency: single C library, musl, static archive.
#
# Extra (host-only) generation flags:
#   - WITH_WIDEC: build the wide-char ncursesw variant Vim's --with-tlib
#     expects (-lncursesw).  Vim's configure checks for tgetent in
#     ncursesw/ncurses/tinfo/etc.
#   - NCURSES_NO_ADA/CXX/MANPAGES/TESTS/PROGS: ncurses tries to build Ada,
#     C++, manpages and test programs by default; all are disabled so only the
#     C library (and the terminfo data that Vim reads at runtime) is produced.

NAME   := ncurses
VERSION:= 6.5
CATEGORY:= base
LICENSE:= X11/MIT (ncurses permissive license)
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Build-time dependency prefix (vim consumes it through NCURSES_PREFIX).
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so this must resolve lazily.
NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/$(NAME)

# Not staged into the rootfs (static archive linked into consumers).
INSTALL_BIN:=
INSTALL_RELPATH:= lib/libncursesw.a

include ../../mk/project-port.mk

# ncurses is C (its own tiny bits of C++/Ada are disabled). Do NOT pull in the
# C++/libc++ link libraries, and keep host make/ar on the toolchain path.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

# The FreeLinX toolchain's llvm-ar/llvm-ranlib require GLIBC_2.38 (host has
# 2.36) and cannot run here; archiving is libc-agnostic so host ar/ranlib
# package the musl .o files.  The final static link uses ONLY FreeLinX
# clang+LLD.
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

# ncurses uses autoconf; point it at the static wide-char build.  The
# --prefix here is the build-time dependency prefix; terminfo data is
# generated into $(NCURSES_PREFIX)/share/terminfo by tic at install time
# (--with-progs builds the terminfo compiler tic, which Vim reads via
# -lncursesw) for manual staging into the rootfs later.
PROJECT_CFG_CMDS = \
	AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)" ./configure \
		--prefix="$(NCURSES_PREFIX)" \
		--host=x86_64-linux-musl \
		--enable-widec \
		--with-normal --without-shared \
		--without-debug --without-ada --without-cxx \
		--without-manpages --without-tests \
		--with-progs --with-terminfo-dirs="$(NCURSES_PREFIX)/share/terminfo:/usr/share/terminfo" \
		--without-cxx-binding --disable-stripping

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install

# The delivered artifact is the static archive (never staged to rootfs).
PROJECT_BIN = $(NCURSES_PREFIX)/lib/libncursesw.a
BUILD_BIN   = $(PROJECT_BIN)