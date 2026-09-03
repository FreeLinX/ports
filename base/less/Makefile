# FreeLinX/ports - base/less : less 668 (terminal pager).
#
# ISC-licensed pager, replacement for GNU less (which is actually the same
# program — less is NOT GNU).  Built with FreeLinX clang + LLD + musl, static.

NAME   := less
VERSION:= 661
CATEGORY:= base
LICENSE:= ISC
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= less
INSTALL_RELPATH:= usr/bin/less

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =-I$(NCURSES_PREFIX)/include
FLX_PROJECT_LDFLAGS  =-L$(NCURSES_PREFIX)/lib

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/ncurses
SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/less-661
PROJECT_BIN = $(SRC_TREE)/less
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	autoreconf -fi && \
	CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(NCURSES_PREFIX)/include -fuse-ld=lld --rtlib=compiler-rt" \
	LDFLAGS="-L$(NCURSES_PREFIX)/lib" \
	LIBS="-lncursesw" \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		CC="$(FREELINX_CC)"

BUILD_CMDS = \
	cd $(SRC_TREE) && \
	sh mkhelp.sh < less.hlp > help.c && \
	grep -h '^public [^;]*$$' *.c | sed 's/$$/;/' > funcs.h && \
	LDFLAGS="-static -fuse-ld=lld --rtlib=compiler-rt" \
	make -j"$(FREELINX_JOBS)"
