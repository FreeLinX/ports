# FreeLinX/ports - devel/nasm : NASM 3.02 (Netwide Assembler for x86).
#
# NASM is the Netwide Assembler for the x86 CPU architecture. Build tool:
# ffmpeg's x86 assembly backends (libx264/libx265 and its own asm) need it
# at build time. 2-clause BSD license. Autoconf build (./configure && make).
#
# Upstream source:
#   https://www.nasm.us/pub/nasm/releasebuilds/3.02/nasm-3.02.tar.xz
# (nasm.us publishes no checksum file for the release; the sha256 in
# distinfo was computed locally from the downloaded tarball.)
#
# Built statically with the FreeLinX clang + LLD + musl toolchain; only the
# toolchain's clang is ever used, never host gcc. Delivers nasm + ndisasm.

NAME    := nasm
VERSION := 3.02
CATEGORY:= devel
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= nasm
INSTALL_RELPATH:= usr/bin/nasm
# The port builds two programs. Without this, ndisasm was staged and never
# installed: install-rootfs copies INSTALL_RELPATH and nothing else.
INSTALL_EXTRA_RELPATHS:= usr/bin/ndisasm

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/nasm-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/nasm
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="$(FLX_LINK_FLAGS)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# nasm builds nasm and ndisasm; the generic rule stages nasm, this one
# stages ndisasm. A stage-extra hook, not an install: override, which would
# lose do-build and fail with "No rule to make target '.../nasm'".
.PHONY: stage-extra
stage-extra:
	@if [ -f "$(SRC_TREE)/ndisasm" ]; then \
		mkdir -p "$(STAGE_ROOT)/usr/bin"; \
		install -m 755 "$(SRC_TREE)/ndisasm" "$(STAGE_ROOT)/usr/bin/ndisasm"; \
	fi
