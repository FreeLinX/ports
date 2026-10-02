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
INSTALL_RELPATH:= lib/libncursestw.a

# ncurses is C (its own tiny bits of C++/Ada are disabled). Do NOT pull in the
# C++/libc++ link libraries, and keep host make/ar on the toolchain path.
# NOTE: these must be set BEFORE the include — project-port.mk simple-expands
# them into the exported CFLAGS/LDFLAGS at include time.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

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
# --enable-reentrant is not optional here, and the default is off.  ncurses 6.5's
# configure says
#
#   if test "${enable_reentrant+set}" = set; then
#     with_reentrant=$enableval
#   else
#     with_reentrant=no
#   fi
#
# and only turns it on by itself for --with-pthreads.  Non-reentrant ncurses
# defines the two global integers
#
#   $ nm --defined-only deps/ncurses/lib/libncursesw.a | grep -E ' B (LINES|COLS)$'
#   0000000000000100 B LINES
#   0000000000000104 B COLS
#
# so every consumer that owns those names collides with it at link time:
#
#   ld.lld: error: duplicate symbol: LINES
#   ld.lld: error: duplicate symbol: COLS
#
# www/w3m is one: terms.c says
#
#   int LINES, COLS;
#
# and drives them itself from the terminfo `lines' and `cols' capabilities.
# Reentrant ncurses renames its copies to lines_reentrant and cols_reentrant, the
# library stops defining the plain names, and the consumer's own definition is
# the only one left.  Reentrant is also what ncurses recommends; the default here
# is only a default.
#
# Reentrant ncurses renames its libraries as well as its globals, and the name is
# libncursestw.a rather than libncursesw.a.  configure.in says so:
#
#   # when not using weak symbols but with_reentrant,
#   # add 't' to the library suffix on all platforms
#
# --disable-lib-suffixes turns that off, but it also turns off the 'w' from
# --enable-widec on the next line of the same block, so the archive becomes
# libncurses.a and the wide one is lost.  There is no switch for w-without-t, so
# the name is libncursestw.a and PROJECT_BIN, base/vim's --with-tlib and
# www/w3m's --with-termlib all say that.
# --datarootdir, and not --with-terminfo, for the database path that ends up in
# every program linking this library.  configure.in has
#
#   if test "$use_database" = no ; then
#       TERMINFO="${datadir}/terminfo"
#
# and ncurses 6.5 has no --with-terminfo option at all: naming one is silently
# ignored and TERMINFO comes from datadir, which is ${datarootdir}.  So
# --prefix alone puts the build tree in it -- include/ncurses_cfg.h came out with
#
#   #define TERMINFO "/home/kanan/FreeLinX/ports/build/deps/ncurses/share/terminfo"
#
# --datarootdir moves datadir to /usr/share while leaving libdir and includedir
# under $prefix, which is what keeps install.includes and install.libs writing
# inside the tree.
#
# --with-terminfo-dirs is the *list* ncurses searches, and is a real option.
# it, and it used to name this build tree:
#
#   --with-terminfo-dirs="$(NCURSES_PREFIX)/share/terminfo:/usr/share/terminfo"
#
# so /home/kanan/FreeLinX/ports/build/deps/ncurses/share/terminfo ended up in the
# strings of the shipped usr/bin/w3m:
#
#   $ strings -a rootfs/usr/bin/w3m | grep terminfo
#   /home/kanan/FreeLinX/ports/build/deps/ncurses/share/terminfo:/usr/share/terminfo
#
# A development path in a shipped binary is not reproducible and is not a path the
# installed system has.  The two directories that are real are /lib/terminfo,
# which the rootfs carries, and /usr/share/terminfo, which is where the ncurses
# port's own tic output is meant to be staged.  The build tree's copy of the
# database is still found during the build, through --prefix and TERMINFO.
PROJECT_CFG_CMDS = \
	AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)" ./configure \
		--prefix="$(NCURSES_PREFIX)" \
		--host=x86_64-linux-musl \
		--enable-widec \
		--enable-reentrant \
		--with-normal --without-shared \
		--without-debug --without-ada --without-cxx \
		--without-manpages --without-tests \
		--with-progs \
		--datarootdir=/usr/share \
		--with-terminfo-dirs=/lib/terminfo:/usr/share/terminfo \
		--without-cxx-binding --disable-stripping

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make -C include install.includes && \
	make -C ncurses install.libs

# The delivered artifact is the static archive (never staged to rootfs).
PROJECT_BIN = $(NCURSES_PREFIX)/lib/libncursestw.a
BUILD_BIN   = $(PROJECT_BIN)

# ncurses has never shipped a pkg-config file.  Two ports in this tree need one:
# devel/readline's readline.pc says
#
#   Requires.private: ncursestw
#
# because that is the only terminfo library here, and www/w3m names ncursestw on
# --with-termlib.  Neither can resolve without this file, and check-deps-pkgconfig
# reports both.
#
# It is written here rather than generated by anything, so every value comes from
# a make variable in this file: the prefix is the dependency tree, the header
# directory is the one include.installs put the headers in (ncursestw/, because
# --disable-lib-suffixes is not in effect and ncurses uses the suffixed name),
# and the version is VERSION above.  Libs names the archive the tree actually has.
NCURSES_PC = $(NCURSES_PREFIX)/lib/pkgconfig/ncursestw.pc

.PHONY: stage-pc
install: stage-pc

stage-pc:
	@mkdir -p "$(dir $(NCURSES_PC))"
	@printf 'prefix=%s\nexec_prefix=$${prefix}\nlibdir=$${exec_prefix}/lib\nincludedir=$${prefix}/include\n\nName: ncursestw\nDescription: wide-character ncurses with terminfo\nURL: https://www.gnu.org/software/ncurses/\nVersion: $(VERSION)\nLibs: -L$${libdir} -lncursestw\nCflags: -I$${includedir}/ncursestw\n' \
		"$(NCURSES_PREFIX)" > "$(NCURSES_PC)"
	@test -f "$(NCURSES_PREFIX)/lib/libncursestw.a" || { \
		printf '[FreeLinX/ports][error] ncurses: libncursestw.a is not in the tree; the module would name a library that is not there\n' >&2; \
		exit 1; \
	}
	@printf '[FreeLinX/ports] staged ncursestw.pc\n'

# The same library under the other two names ncurses is normally installed
# under, and the headers under an ncurses/ subdirectory.
#
# Not every consumer takes the library name as a variable.  sysutils/htop's
# configure.ac walks a fixed list and gives up when none of them links:
#
#   HTOP_CHECK_LIB([ncursesw6], [addnwstr], ...)
#    HTOP_CHECK_LIB([ncursesw],  [addnwstr], ...)
#     HTOP_CHECK_LIB([ncurses],  [addnwstr], ...)
#      AC_MSG_ERROR([can not find required library libncursesw; you may want to
#                   use --disable-unicode])
#
# and with --disable-unicode it walks the narrow list instead -lncurses6,
# -lncurses, -lcurses - and stops at
#
#   checking for doupdate in -lncurses6... no
#   checking for doupdate in -lncurses... no
#   checking for doupdate in -lcurses... no
#   configure: error: can not find required curses/ncurses library
#
# sysutils/ncdu's generated Makefile says the same in a variable:
#
#   LIBS        =  -lncursesw
#   NCURSES_LIBS = -lncursesw
#
# Both would need a patch to say ncursestw.  Symlinks are the smaller change
# and they cannot go stale, because they point at the archive this same file
# just built.  The ncurses/ include directory is the same idea for htop's
# AC_CHECK_HEADERS([ncursesw/curses.h]) and friends.
#
# Nothing links two of these at once -- configure probes and picks one -- so
# the alias cannot cause a duplicate archive on a link line.
.PHONY: stage-names
install: stage-names

NCURSES_LIB_ALIASES = libncurses.a libncursesw.a libcurses.a
NCURSES_HDR_ALIASES = curses.h ncurses.h term.h termcap.h unctrl.h ncurses_dll.h term_entry.h

stage-names:
	@set -e; \
	for l in $(NCURSES_LIB_ALIASES); do \
		ln -sf libncursestw.a "$(NCURSES_PREFIX)/lib/$$l"; \
	done; \
	mkdir -p "$(NCURSES_PREFIX)/include/ncurses"; \
	for h in $(NCURSES_HDR_ALIASES); do \
		ln -sf ../ncursestw/$$h "$(NCURSES_PREFIX)/include/ncurses/$$h"; \
	done; \
	for l in $(NCURSES_LIB_ALIASES); do \
		test -f "$(NCURSES_PREFIX)/lib/$$l" || { \
			printf '[FreeLinX/ports][error] ncurses: %s is not a link to libncursestw.a\n' "$$l" >&2; \
			exit 1; \
		}; \
	done; \
	printf '[FreeLinX/ports] staged ncurses aliases: %s\n' "$(NCURSES_LIB_ALIASES)"
