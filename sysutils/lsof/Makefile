# FreeLinX/ports - sysutils/lsof : lsof 4.99.3 (list open files).
#
# Lists open files for running processes. Uses lsof's custom Configure
# script (not autoconf). BSD-3-Clause license.
# NOTE: Using GitHub source archive; release tarball (.tar.lz) 404'd.

NAME    := lsof
VERSION := 4.99.3
CATEGORY:= sysutils
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= lsof
INSTALL_RELPATH:= usr/bin/lsof

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/lsof-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/lsof
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	LSOF_INCLUDE="$(FREELINX_SYSROOT)/include" \
	LSOF_CC="$(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS)" \
	LINUX_INCL="$(FREELINX_SYSROOT)/include" \
	LINUX_HASSELINUX=no \
	./Configure linux

BUILD_CMDS = \
	$(MAKE) -j"$(FREELINX_JOBS)" \
		CFGL="-static -fuse-ld=lld --rtlib=compiler-rt -L./lib -llsof"
