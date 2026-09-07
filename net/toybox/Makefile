# FreeLinX/ports - net/toybox : toybox 0.8.11 (multi-call binary toolkit).
#
# Lightweight multi-call binary toolkit. BSD-0-Clause (Zero Clause BSD).
# Plain Makefile build with defconfig.

NAME    := toybox
VERSION := 0.8.11
CATEGORY:= net
LICENSE := BSD-0-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= toybox
INSTALL_RELPATH:= usr/bin/toybox

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/toybox-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/toybox
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) defconfig CC="$(FREELINX_CC)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt"
