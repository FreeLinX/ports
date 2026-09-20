# FreeLinX/ports - games/robots : NetBSD 10.1 `/games/robots`.
#
# Builds the ACTUAL NetBSD source (games/robots/) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), statically linked.
NAME   := robots
VERSION:= 10.1
CATEGORY:= games
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= robots
INSTALL_RELPATH:= usr/games/robots

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/robots

NETBSD_MEMBERS = usr/src/games/robots/auto.c \
	usr/src/games/robots/extern.c \
	usr/src/games/robots/flush_in.c \
	usr/src/games/robots/init_field.c \
	usr/src/games/robots/main.c \
	usr/src/games/robots/make_level.c \
	usr/src/games/robots/move.c \
	usr/src/games/robots/move_robs.c \
	usr/src/games/robots/pathnames.h \
	usr/src/games/robots/play_level.c \
	usr/src/games/robots/query.c \
	usr/src/games/robots/rnd_pos.c \
	usr/src/games/robots/robots.h \
	usr/src/games/robots/score.c

PORT_SRCS = games/robots/auto.c \
	games/robots/extern.c \
	games/robots/flush_in.c \
	games/robots/init_field.c \
	games/robots/main.c \
	games/robots/make_level.c \
	games/robots/move.c \
	games/robots/move_robs.c \
	games/robots/play_level.c \
	games/robots/query.c \
	games/robots/rnd_pos.c \
	games/robots/score.c
FLX_CPPFLAGS += $(FLX_NCURSES_CPPFLAGS)
FLX_LDADD    += $(FLX_NCURSES)/lib/libncursesw.a
FLX_CPPFLAGS += -DMAX_PER_UID=5 -DFANCY
