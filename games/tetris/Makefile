# FreeLinX/ports - games/tetris : NetBSD 10.1 tetris game.
#
# Terminal-based Tetris game from NetBSD.  BSD license.
# Builds the ACTUAL NetBSD tetris source (games/tetris/) with the FreeLinX
# toolchain (clang + LLD + musl sysroot), statically linked.
#
# FreeLinX porting notes:
#   - Uses ncursesw from build/deps/ncurses for <term.h> and terminfo(3).
#   - FLX_NCURSES_CPPFLAGS / FLX_NCURSES_LDADD come from base-port.mk.
#   - pathnames.h hardcodes _PATH_SCOREFILE=/var/games/tetris.scores.

NAME   := tetris
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tetris
INSTALL_RELPATH:= usr/games/tetris

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/tetris

NETBSD_MEMBERS = usr/src/games/tetris/input.c usr/src/games/tetris/input.h \
	usr/src/games/tetris/pathnames.h usr/src/games/tetris/scores.c \
	usr/src/games/tetris/scores.h usr/src/games/tetris/screen.c \
	usr/src/games/tetris/screen.h usr/src/games/tetris/shapes.c \
	usr/src/games/tetris/tetris.c usr/src/games/tetris/tetris.h

PORT_SRCS = games/tetris/input.c games/tetris/scores.c \
	games/tetris/screen.c games/tetris/shapes.c games/tetris/tetris.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/arc4random.c

# tetris needs ncursesw/terminfo for <term.h>
FLX_CPPFLAGS += $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD    += $(FLX_NCURSES_LDADD)
