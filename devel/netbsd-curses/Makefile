# FreeLinX/ports - devel/netbsd-curses : NetBSD curses library (0.3.2).
#
# Portable NetBSD curses (sabotage): libcurses, libterminfo, libform,
# libmenu, libpanel, wide-char like ncursesw.  BSD license.  This is the
# curses every FreeLinX port links against; GNU ncurses is not used.
#
# Installed into $(FREELINX_BUILD_DIR)/deps/netbsd-curses with the names
# ncurses-based build systems look for (libncursesw.a, libtinfo.a,
# include/ncursesw/curses.h, ncursesw.pc), so ports build unchanged.
# The terminfo database is compiled to share/terminfo.cdb (installed on
# the system as /usr/share/terminfo.cdb); xterm, linux, vt100 and friends
# are also compiled into libterminfo itself.

NAME    := netbsd-curses
VERSION := 0.3.2
CATEGORY:= devel
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= lib/libcurses.a

include ../../mk/project-port.mk

NC_PREFIX = $(FREELINX_BUILD_DIR)/deps/netbsd-curses
NC_MAKE = $(MAKE) -f GNUmakefile HOSTCC=cc CC="$(FREELINX_CC)" AR="$(FREELINX_AR)" \
	RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
	CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -fPIC -DTERMINFO_COMPAT" \
	CFLAGS_HOST="-O2 -DTERMINFO_COMPAT" \
	LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt" \
	PREFIX=/usr LIBDIR=$(NC_PREFIX)/lib INCDIR=$(NC_PREFIX)/include

BUILD_CMDS = \
	$(NC_MAKE) -j"$(FREELINX_JOBS)" all-static && \
	$(NC_MAKE) terminfo/terminfo.cdb && \
	rm -rf "$(NC_PREFIX)" && \
	$(NC_MAKE) install-headers install-stalibs install-pcs && \
	ln -sf libterminfo.a "$(NC_PREFIX)/lib/libtinfo.a" && \
	ln -sf libterminfo.a "$(NC_PREFIX)/lib/libtinfow.a" && \
	ln -sf libcurses.a "$(NC_PREFIX)/lib/libcursesw.a" && \
	ln -sf . "$(NC_PREFIX)/include/ncursesw" && \
	ln -sf . "$(NC_PREFIX)/include/ncurses" && \
	mkdir -p "$(NC_PREFIX)/share" "$(NC_PREFIX)/bin" && \
	cp terminfo/terminfo.cdb "$(NC_PREFIX)/share/terminfo.cdb" && \
	for p in tic/tic tput/tput tset/tset infocmp/infocmp tabs/tabs; do \
		cp "$$p" "$(NC_PREFIX)/bin/"; done && \
	cp tput/clear.sh "$(NC_PREFIX)/bin/clear"

PROJECT_BIN = $(NC_PREFIX)/lib/libcurses.a
