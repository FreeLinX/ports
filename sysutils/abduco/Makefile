# FreeLinX/ports - sysutils/abduco : session manager (git snapshot).
#
# Simple session manager allowing programs to be started detached.
# ISC license. POSIX make build.

NAME    := abduco
VERSION := 0.6
CATEGORY:= sysutils
LICENSE := ISC
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= abduco
INSTALL_RELPATH:= usr/bin/abduco

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/abduco-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/abduco

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/config.mk"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -DNDEBUG -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt" \
		LDFLAGS_STD="" \
		abduco
