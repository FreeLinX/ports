# FreeLinX/ports - sysutils/file : file 5.46 (file type identification).
#
# Determines file types by examining file content. Autoconf build.
# BSD-2-Clause license. No zlib dependency (--with-zlib=no).

NAME    := file
VERSION := 5.46
CATEGORY:= sysutils
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= file
INSTALL_RELPATH:= usr/bin/file

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/file-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/file
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		--with-zlib=no \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	$(MAKE) -j"$(FREELINX_JOBS)" FILE_COMPILE=/usr/bin/file
