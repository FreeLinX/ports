# FreeLinX/ports - textproc/mawk : mawk 1.3.4-20240123 (AWK interpreter).
#
# mawk is an interpreter for the AWK programming language, written by Mike
# Brennan and maintained by Thomas E. Dickey. GPL-2.0 licensed.
#
# Build model: plain Makefile with a configure script. Built against the
# FreeLinX clang + LLD + musl toolchain, statically linked with lld.
#
# NOTE ON SOURCE/SHA256: The initially-requested archive
# "mawk-20240122.tgz" (and URL .../archives/mawk/mawk-20240122.tgz) returns
# HTTP 404 and does not exist upstream. The real upstream archive matching the
# intended 2024-01 version is mawk-1.3.4-20240123.tgz (top dir
# mawk-1.3.4-20240123), whose SHA256 was computed from a real download:
#   a8e319a83744b1f1fb6988dfa189d61887f866e9140cc9a49eb003b2b0655e88
# This port therefore records DISTINFO_NAME=mawk-1.3.4-20240123 and the real
# hash. TODO: revisit if archives/<mawk-20240122.tgz> ever appears upstream.

NAME    := mawk
VERSION := 1.3.4-20240123
CATEGORY:= textproc
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= mawk
INSTALL_RELPATH:= usr/bin/mawk

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/mawk-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/mawk
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2"

BUILD_CMDS = \
	make CC="$(FREELINX_CC)" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
		mawk
