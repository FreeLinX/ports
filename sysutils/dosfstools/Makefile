# FreeLinX/ports - sysutils/dosfstools : dosfstools 4.2 (FAT filesystem tools).
#
# Provides mkfs.fat, fsck.fat, and fatlabel for FAT12/FAT16/FAT32
# filesystem management. Essential for EFI System Partitions and USB media.

NAME    := dosfstools
VERSION := 4.2
CATEGORY:= sysutils
LICENSE := GPL-3.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= mkfs.fat
INSTALL_RELPATH:= sbin/mkfs.fat
# The other binaries, staged by stage-extra and copied by install-rootfs.
INSTALL_EXTRA_RELPATHS:= sbin/fsck.fat sbin/fatlabel

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dosfstools-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/mkfs.fat
PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--sbindir=/sbin \
		--enable-static \
		--disable-shared \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# dosfstools produces 3 binaries: mkfs.fat, fsck.fat, fatlabel
# Stages the other two binaries dosfstools builds.  This used to be an
# `install:` override, which loses do-build and made `make install`
# fail with "No rule to make target '.../src/mkfs.fat'" -- nothing had
# built anything.  An `install-rootfs:` override alongside it earned two
# make warnings about overriding the rule that actually runs.
.PHONY: stage-extra
stage-extra:
	@if [ -f "$(SRC_TREE)/src/fsck.fat" ]; then \
		mkdir -p "$(STAGE_ROOT)/sbin"; \
		install -m 755 "$(SRC_TREE)/src/fsck.fat" "$(STAGE_ROOT)/sbin/fsck.fat"; \
	fi
	@if [ -f "$(SRC_TREE)/src/fatlabel" ]; then \
		mkdir -p "$(STAGE_ROOT)/sbin"; \
		install -m 755 "$(SRC_TREE)/src/fatlabel" "$(STAGE_ROOT)/sbin/fatlabel"; \
	fi
