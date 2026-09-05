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

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dosfstools-$(VERSION)
BUILD_BIN = $(SRC_TREE)/src/mkfs.fat

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--sbindir=/sbin \
		--enable-static \
		--disable-shared \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# dosfstools produces 3 binaries: mkfs.fat, fsck.fat, fatlabel
install: $(STAGE_FILE)
	@if [ -f "$(SRC_TREE)/src/fsck.fat" ]; then \
		mkdir -p "$(STAGE_ROOT)/sbin"; \
		install -m 755 "$(SRC_TREE)/src/fsck.fat" "$(STAGE_ROOT)/sbin/fsck.fat"; \
	fi
	@if [ -f "$(SRC_TREE)/src/fatlabel" ]; then \
		mkdir -p "$(STAGE_ROOT)/sbin"; \
		install -m 755 "$(SRC_TREE)/src/fatlabel" "$(STAGE_ROOT)/sbin/fatlabel"; \
	fi

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@if [ -f "$(SRC_TREE)/src/fsck.fat" ]; then \
		mkdir -p "$(ROOTFS_DIR)/sbin"; \
		install -m 755 "$(SRC_TREE)/src/fsck.fat" "$(ROOTFS_DIR)/sbin/fsck.fat"; \
	fi
	@if [ -f "$(SRC_TREE)/src/fatlabel" ]; then \
		mkdir -p "$(ROOTFS_DIR)/sbin"; \
		install -m 755 "$(SRC_TREE)/src/fatlabel" "$(ROOTFS_DIR)/sbin/fatlabel"; \
	fi
