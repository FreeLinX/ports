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
# Staged by stage-extra, copied by install-rootfs.  fsck, mkfs and blkid
# are named here as well as the symlinks: the framework copies a path, and a
# symlink whose target is not copied is a dangling pointer in the rootfs.
INSTALL_EXTRA_RELPATHS:= sbin/e2fsck sbin/tune2fs sbin/dumpe2fs sbin/resize2fs \
	sbin/fsck sbin/fsck.ext2 sbin/fsck.ext3 sbin/fsck.ext4 \
	sbin/mkfs.ext2 sbin/mkfs.ext3 sbin/mkfs.ext4

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/e2fsprogs-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/misc/mke2fs
# --disable-fuse2fs: fuse2fs mounts an ext filesystem through libfuse, which
# this target does not have and musl has no usable headers for.  It is also
# the one program in the package that does not compile against musl:
#
#   fuse2fs.c:4405:10: error: cannot assign to variable 'stderr' with
#       const-qualified type 'FILE *const'
#   fuse2fs.c:4406:10: error: cannot assign to variable 'stdout' with
#       const-qualified type 'FILE *const'
#
# musl declares stdout and stderr as `extern FILE *const`, so an assignment
# to them is an error, and e2fsprogs 1.47.3's fuse2fs redirects them.  The
# other twenty-odd tools in the package build and install cleanly, and the
# installer needs mke2fs and e2fsck, not fuse2fs.
PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--sbindir=/sbin \
		--bindir=/usr/bin \
		--enable-static \
		--disable-shared \
		--enable-libuuid \
		--disable-fuse2fs \
		--enable-libblkid \
		--disable-testio-debug \
		--disable-backtrace \
		--disable-debugfs-progs \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# e2fsprogs produces many binaries. Stage the critical ones:
# mke2fs (mkfs.ext4), e2fsck (fsck.ext4), tune2fs, dumpe2fs, resize2fs
# Stages the other binaries, as stage-extra rather than as an `install:`
# override.  Overriding `install:` loses do-build, so `make install` here never
# built anything and then failed on
#
#   make: *** No rule to make target '.../misc/mke2fs'
#
# and the matching `install-rootfs:` override earned two make warnings about
# overriding the rule that actually runs.  The symlinks are still made here, so
# they exist in staging before install-rootfs copies INSTALL_EXTRA_RELPATHS.
.PHONY: stage-extra
# Each binary is named with the subdirectory e2fsprogs builds it in, because they
# are not all in misc/: e2fsck comes from e2fsck/ and resize2fs from resize/.
#
# The previous loop looked for all four in misc/ behind an `if [ -f ... ]`, so it
# silently staged neither e2fsck nor resize2fs.  The guard turned a wrong path
# into a missing program rather than an error, and the sbin/e2fsck and
# sbin/resize2fs that were in the rootfs were committed prebuits, not port builds.
# The check is now a hard failure: in this package a missing binary means the path
# is wrong, not that the program is optional.
#
# The symlinks are made here too, so they exist in staging before install-rootfs
# copies INSTALL_EXTRA_RELPATHS -- a symlink whose target has not been copied is
# a dangling pointer in the rootfs, and setup-disk.sh runs fsck.ext4 and
# mkfs.ext4 by name.
stage-extra:
	@for _e in e2fsck:e2fsck tune2fs:misc resize2fs:resize dumpe2fs:misc \
	           badblocks:misc blkid:misc chattr:misc lsattr:misc fsck:misc; do \
	    _b=$${_e%%:*}; _d=$${_e##*:}; \
	    if [ ! -f "$(SRC_TREE)/$$_d/$$_b" ]; then \
	        printf '[FreeLinX/ports][error] %s: %s is not at %s/%s in the build tree\n' \
	            "$(NAME)" "$$_b" "$$_d" "$$_b" >&2; \
	        exit 1; \
	    fi; \
	    $(MKDIR) -p "$(STAGE_ROOT)/sbin"; \
	    $(INSTALL) -m 755 "$(SRC_TREE)/$$_d/$$_b" "$(STAGE_ROOT)/sbin/$$_b"; \
	done
	@for _a in mkfs.ext2 mkfs.ext3 mkfs.ext4; do \
	    ln -sf mke2fs "$(STAGE_ROOT)/sbin/$$_a"; \
	done
	@for _a in fsck.ext2 fsck.ext3 fsck.ext4; do \
	    ln -sf e2fsck "$(STAGE_ROOT)/sbin/$$_a"; \
	done
