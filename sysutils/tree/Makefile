# FreeLinX/ports - sysutils/tree : tree 2.1.3 (directory listing utility).
#
# Displays directory tree structures. Simple POSIX make build, no autoconf.
# GPL-2.0 license.

NAME    := tree
VERSION := 2.1.3
CATEGORY:= sysutils
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= tree
INSTALL_RELPATH:= usr/bin/tree

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/tree-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/tree
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -D_GNU_SOURCE" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
		tree
