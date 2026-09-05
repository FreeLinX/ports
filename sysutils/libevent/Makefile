# FreeLinX/ports - sysutils/libevent : libevent 2.1.12 (event notification).
#
# Asynchronous event notification library. Build dependency for tmux.

NAME    := libevent
VERSION := 2.1.12
CATEGORY:= sysutils
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= lib/libevent.a

LIBEVENT_PREFIX = $(FREELINX_BUILD_DIR)/deps/libevent

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libevent-$(VERSION)-stable
PROJECT_BIN = $(LIBEVENT_PREFIX)/lib/libevent.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix="$(LIBEVENT_PREFIX)" \
		--enable-static \
		--disable-shared \
		--disable-openssl \
		--disable-debug-mode \
		--disable-dependency-tracking \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install
