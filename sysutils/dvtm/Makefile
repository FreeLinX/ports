# FreeLinX/ports - sysutils/dvtm : dvtm 0.15 (dynamic virtual terminal manager).
#
# Dynamic virtual terminal manager for the console. Plain Makefile build.
# MIT license. Requires curses (devel/netbsd-curses).

NAME    := dvtm
VERSION := 0.15
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:= netbsd-curses
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), netbsd-curses (devel/netbsd-curses)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dvtm
INSTALL_RELPATH:= usr/bin/dvtm

NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/netbsd-curses

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dvtm-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/dvtm
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && printf '\#define VERSION "%s"\n' "$(VERSION)" > flx-version.h && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CPPFLAGS="-D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED" \
		dvtm
