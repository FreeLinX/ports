# FreeLinX/ports - base/libarchive : libarchive 3.8.9 (bsdtar + bsdcpio).
#
# Real upstream libarchive built with the FreeLinX clang + LLD + musl toolchain,
# fully static.  libarchive is the reference implementation of bsdtar used by
# FreeBSD, NetBSD, and macOS.  It produces:
#   - bsdtar  (staged as /bin/tar — the system tar)
#   - bsdcpio (staged as /bin/cpio)
#   - bsdcat  (staged as /bin/bsdcat)
#   - libarchive.a (build-time dependency, not staged to rootfs)
#
# Dependencies: zlib (base/zlib, already ported, optional but recommended).
# Optional deps NOT enabled: liblzma, libbz2, libzstd (add later as needed).
#
# No GNU runtime dependency.

NAME   := libarchive
VERSION:= 3.8.9
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl, zlib (base/zlib)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Primary binary — bsdtar is the critical deliverable.
INSTALL_BIN:= bsdtar
INSTALL_RELPATH:= bin/tar

# Build-time dependency prefix for zlib (already built by base/zlib).
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

# libarchive is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

# Host ar/ranlib (same GLIBC_2.38 caveat as openssl/zlib).
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/bsdtar
BUILD_BIN   = $(PROJECT_BIN)

# libarchive uses autoconf configure.  Point it at the FreeLinX zlib and
# disable optional deps we haven't ported yet.
PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--enable-bsdtar=static --enable-bsdcpio=static --enable-bsdcat=static \
		--with-zlib \
		--without-bz2lib --without-libb2 --without-lzma \
		--without-lz4 --without-zstd --without-cng \
		--without-openssl --without-xml2 --without-expat \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(ZLIB_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(ZLIB_PREFIX)/lib" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# Multi-binary port: bsdtar is primary (staged by install.mk).
# bsdcpio and bsdcat are staged by extra install rules below.
install: $(STAGE_FILE)
	@if [ -f "$(SRC_TREE)/bsdcpio" ]; then \
		printf '[FreeLinX/ports] staging bsdcpio -> %s/bin/cpio\n' "$(STAGE_ROOT)"; \
		mkdir -p "$(STAGE_ROOT)/bin"; \
		install -m 755 "$(SRC_TREE)/bsdcpio" "$(STAGE_ROOT)/bin/cpio"; \
	fi
	@if [ -f "$(SRC_TREE)/bsdcat" ]; then \
		printf '[FreeLinX/ports] staging bsdcat -> %s/bin/bsdcat\n' "$(STAGE_ROOT)"; \
		install -m 755 "$(SRC_TREE)/bsdcat" "$(STAGE_ROOT)/bin/bsdcat"; \
	fi

install-rootfs: check-install-rootfs $(STAGE_FILE)
	@printf '[FreeLinX/ports] installing %s into rootfs %s\n' "$(NAME)" "$(ROOTFS_FILE)"
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@if [ -f "$(STAGE_ROOT)/bin/cpio" ]; then \
		mkdir -p "$(ROOTFS_DIR)/bin"; \
		install -m 755 "$(STAGE_ROOT)/bin/cpio" "$(ROOTFS_DIR)/bin/cpio"; \
	fi
	@if [ -f "$(STAGE_ROOT)/bin/bsdcat" ]; then \
		install -m 755 "$(STAGE_ROOT)/bin/bsdcat" "$(ROOTFS_DIR)/bin/bsdcat"; \
	fi
