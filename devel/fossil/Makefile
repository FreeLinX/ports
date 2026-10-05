# FreeLinX/ports - devel/fossil : fossil 2.28 (distributed version control).
#
# Fossil is a self-contained, single-binary distributed version control,
# bug tracking, wiki and forum system with a built-in web UI.  It bundles
# its own SQLite engine; zlib comes from base/zlib and links statically,
# so the delivered binary has no runtime dependencies beyond the kernel.
# HTTPS sync is disabled (--with-openssl=none) to keep the port free of
# further dependencies; point --with-openssl at build/deps/openssl and
# add base/openssl to BUILD_DEPENDENCIES to enable it.
#
# BSD-2-Clause license.

NAME    := fossil
VERSION := 2.28
CATEGORY:= devel
LICENSE := BSD-2-Clause
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), zlib (base/zlib, statically linked)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= fossil
INSTALL_RELPATH:= usr/bin/fossil

# Build-time dependency prefix produced by base/zlib.
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so this must resolve lazily.
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/fossil-src-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/fossil
BUILD_BIN   = $(PROJECT_BIN)

# Autosetup configure (not GNU autoconf): toolchain arrives through the
# environment the way www/links does it, and feature selection through
# configure flags.  LDFLAGS is driver-managed (no -nostdlib): like links,
# the FreeLinX clang driver appends the musl crt objects and -lc after
# fossil's own -L/-l flags, so the -lz from --with-zlib resolves against
# base/zlib's static archive.  --static matches the ports static-link
# convention; SQLite stays internal (bundled).
PROJECT_CFG_CMDS = \
	CC="$(FREELINX_CC)" \
	CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -I$(ZLIB_PREFIX)/include" \
	LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(ZLIB_PREFIX)/lib" \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--static \
		--disable-fusefs \
		--with-openssl=none \
		--with-zlib="$(ZLIB_PREFIX)"

# BUILD_CMDS overrides BCC/BCCFLAGS: fossil builds its code generators
# (translate, makeheaders, ...) with the build-host compiler, but configure
# bakes the cross CFLAGS (--target/--sysroot) into BCCFLAGS, under which
# host GCC fails, and FreeLinX clang with compile-only flags links through
# the host /usr/bin/ld.  Pointing BCC at the FreeLinX clang with a
# driver-managed static link (the same shape as LDFLAGS above) keeps one
# consistent set of flags; the generators come out as static musl x86_64
# binaries, which execute on the build host like any other FreeLinX output.
BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" \
		BCC="$(FREELINX_CC)" \
		BCCFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 --rtlib=compiler-rt -static -fuse-ld=lld"
