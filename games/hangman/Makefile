# FreeLinX/ports - games/hangman : NetBSD 10.1 `/games/hangman`.
#
# Builds the ACTUAL NetBSD source (games/hangman/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := hangman
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= hangman
INSTALL_RELPATH:= usr/games/hangman

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/hangman

NETBSD_MEMBERS = usr/src/games/hangman/endgame.c \
	usr/src/games/hangman/extern.c \
	usr/src/games/hangman/getguess.c \
	usr/src/games/hangman/getword.c \
	usr/src/games/hangman/hangman.h \
	usr/src/games/hangman/main.c \
	usr/src/games/hangman/pathnames.h \
	usr/src/games/hangman/playgame.c \
	usr/src/games/hangman/prdata.c \
	usr/src/games/hangman/prman.c \
	usr/src/games/hangman/prword.c \
	usr/src/games/hangman/setup.c

PORT_SRCS = games/hangman/endgame.c \
	games/hangman/extern.c \
	games/hangman/getguess.c \
	games/hangman/getword.c \
	games/hangman/main.c \
	games/hangman/playgame.c \
	games/hangman/prdata.c \
	games/hangman/prman.c \
	games/hangman/prword.c \
	games/hangman/setup.c
FLX_CPPFLAGS += $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD    += $(FLX_NCURSES)/lib/libncursesw.a
