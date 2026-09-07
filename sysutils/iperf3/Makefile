# FreeLinX/ports - sysutils/iperf3 : iperf 3.17.1 (network bandwidth tool).
#
# Cross-platform TCP/UDP bandwidth measurement tool. Autoconf build.
# BSD-3-Clause license.

NAME    := iperf3
VERSION := 3.17.1
CATEGORY:= sysutils
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= iperf3
INSTALL_RELPATH:= usr/bin/iperf3

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/iperf-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/iperf3
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
