# FreeLinX/ports - sysutils/tmux : tmux 3.4 (terminal multiplexer).
#
# tmux allows multiple terminal sessions to be accessed from a single
# terminal. Depends on libevent (build dependency) and ncurses (via musl).

NAME    := tmux
VERSION := 3.4
CATEGORY:= sysutils
LICENSE := BSD-2-Clause
DEPENDENCIES:= libevent
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), libevent (sysutils/libevent)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= tmux
INSTALL_RELPATH:= usr/bin/tmux

LIBEVENT_PREFIX = $(FREELINX_BUILD_DIR)/deps/libevent
NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/ncurses

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/tmux-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/tmux
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	ac_cv_search_forkpty="none required" \
	ac_cv_search_fdforkpty=no \
	./configure \
		--prefix=/usr \
		--enable-static \
		--disable-shared \
		CPPFLAGS="-I$(LIBEVENT_PREFIX)/include -I$(NCURSES_PREFIX)/include -I$(NCURSES_PREFIX)/include/ncursesw" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(LIBEVENT_PREFIX)/lib -L$(NCURSES_PREFIX)/lib" \
		CC="$(FREELINX_CC)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
