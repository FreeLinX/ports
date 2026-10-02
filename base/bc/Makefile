# FreeLinX/ports - base/bc : bc 7.0.3 (Gavin D. Howard).
#
# An independent bc (BSD-2-Clause; FreeBSD's bc since 13.0), replacing the
# NetBSD bc, which is a BSD-relicensed descendant of GNU bc.  POSIX bc with
# the GNU extensions, plus the math library (-l).  Static, no NLS, no line
# editing.  Its own configure.sh drives the build; the string generator it
# runs at build time is a static musl binary like everything else here.

NAME    := bc
VERSION := 7.0.3
CATEGORY:= base
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= bc
INSTALL_RELPATH:= usr/bin/bc

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/bc-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/bin/bc

BC_CC = $(FREELINX_CC) --target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE)

PROJECT_CFG_CMDS = \
	CC="$(BC_CC)" HOSTCC="$(BC_CC) -static -fuse-ld=lld --rtlib=compiler-rt" \
	CFLAGS="-O2" LDFLAGS="-static -fuse-ld=lld --rtlib=compiler-rt" PREFIX=/usr \
	./configure.sh -O2 -N -G -l

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
