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


PROJECT_BIN = $(NC_PREFIX)/lib/libcurses.a
