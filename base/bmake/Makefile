# FreeLinX/ports - base/bmake : NetBSD bmake (portable BSD make).
#
# Portable NetBSD make utility built with FreeLinX clang + LLD + musl.
# Used by BSD ports and BSD Makefiles. Staged as /usr/bin/bmake.

NAME   := bmake
VERSION:= portable
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), sh (host)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= bmake
INSTALL_RELPATH:= usr/bin/bmake

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/bmake
PROJECT_BIN = $(SRC_TREE)/bmake
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--with-default-sys-path=/usr/share/mk \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	sh ./make-bootstrap.sh
