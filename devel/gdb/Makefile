# FreeLinX/ports - devel/gdb : GNU Debugger 15.2.
#
# GDB, the GNU Project debugger, lets you see what is going on inside a program
# while it executes. GPL-3.0 licensed.
#
# Build model: autoconf configure (like pcre2/curl). Built fully static against
# the FreeLinX clang + LLD + musl toolchain. Python, Guile and the TUI are
# disabled; gdbserver is not built. This builds the standalone native debugger
# binary gdb/gdb.

NAME    := gdb
VERSION := 15.2
CATEGORY:= devel
LICENSE := GPL-3.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= gdb
INSTALL_RELPATH:= usr/bin/gdb

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/gdb-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/gdb/gdb
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		--disable-werror \
		--without-python \
		--without-guile \
		--without-tui \
		--disable-gdbserver \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
