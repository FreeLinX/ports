# FreeLinX/ports - sysutils/e2fsprogs : e2fsprogs 1.47.3 (ext2/3/4 tools).
#
# Provides mkfs.ext4, fsck.ext4, tune2fs, dumpe2fs, resize2fs, debugfs.
# Essential for creating and maintaining ext2/3/4 filesystems.
# Uses GitHub source archive (no release tarballs on GitHub releases).

NAME    := e2fsprogs
VERSION := 1.47.3
CATEGORY:= sysutils
LICENSE := LGPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= mke2fs
INSTALL_RELPATH:= sbin/mke2fs

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/e2fsprogs-$(VERSION)
BUILD_BIN = $(SRC_TREE)/misc/mke2fs

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--sbindir=/sbin \
		--bindir=/usr/bin \
		--enable-static \
		--disable-shared \
		--enable-libuuid \
		--enable-libblkid \
		--disable-testio-debug \
		--disable-backtrace \
		--disable-debugfs-progs \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# e2fsprogs produces many binaries. Stage the critical ones:
# mke2fs (mkfs.ext4), e2fsck (fsck.ext4), tune2fs, dumpe2fs, resize2fs
install: $(STAGE_FILE)
	@for bin in e2fsck tune2fs dumpe2fs resize2fs; do \
		if [ -f "$(SRC_TREE)/misc/$$bin" ]; then \
			mkdir -p "$(STAGE_ROOT)/sbin"; \
			install -m 755 "$(SRC_TREE)/misc/$$bin" "$(STAGE_ROOT)/sbin/$$bin"; \
		fi; \
	done
	@# Create standard symlinks for mke2fs
	@cd "$(STAGE_ROOT)/sbin" && ln -sf mke2fs mkfs.ext2 2>/dev/null || true
	@cd "$(STAGE_ROOT)/sbin" && ln -sf mke2fs mkfs.ext3 2>/dev/null || true
	@cd "$(STAGE_ROOT)/sbin" && ln -sf mke2fs mkfs.ext4 2>/dev/null || true
	@# Create standard symlinks for e2fsck
	@cd "$(STAGE_ROOT)/sbin" && ln -sf e2fsck fsck.ext2 2>/dev/null || true
	@cd "$(STAGE_ROOT)/sbin" && ln -sf e2fsck fsck.ext3 2>/dev/null || true
	@cd "$(STAGE_ROOT)/sbin" && ln -sf e2fsck fsck.ext4 2>/dev/null || true

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@for bin in e2fsck tune2fs dumpe2fs resize2fs; do \
		if [ -f "$(SRC_TREE)/misc/$$bin" ]; then \
			mkdir -p "$(ROOTFS_DIR)/sbin"; \
			install -m 755 "$(SRC_TREE)/misc/$$bin" "$(ROOTFS_DIR)/sbin/$$bin"; \
		fi; \
	done
	@cd "$(ROOTFS_DIR)/sbin" && ln -sf mke2fs mkfs.ext2 2>/dev/null || true
	@cd "$(ROOTFS_DIR)/sbin" && ln -sf mke2fs mkfs.ext3 2>/dev/null || true
	@cd "$(ROOTFS_DIR)/sbin" && ln -sf mke2fs mkfs.ext4 2>/dev/null || true
	@cd "$(ROOTFS_DIR)/sbin" && ln -sf e2fsck fsck.ext2 2>/dev/null || true
	@cd "$(ROOTFS_DIR)/sbin" && ln -sf e2fsck fsck.ext3 2>/dev/null || true
	@cd "$(ROOTFS_DIR)/sbin" && ln -sf e2fsck fsck.ext4 2>/dev/null || true
