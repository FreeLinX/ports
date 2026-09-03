# FreeLinX/ports - base/mandoc : mandoc 1.14.6 (man page formatter/viewer).
#
# BSD-2-Clause, from OpenBSD.  Replaces GNU groff + man.
# Built with FreeLinX clang + LLD + musl, static.

NAME   := mandoc
VERSION:= 1.14.6
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mandoc
INSTALL_RELPATH:= bin/mandoc

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/mandoc-1.14.6
PROJECT_BIN = $(SRC_TREE)/mandoc
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--host=$(FREELINX_TRIPLE) \
		--enable-static \
		--disable-shared \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
