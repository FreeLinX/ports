# FreeLinX/ports - base/sqlite : SQLite 3.45.3 (build dependency, upstream).

# SQLite is a build-time dependency for xpkg (the FreeLinX package
# manager's installed-package database). Built with the FreeLinX clang +
# LLD + musl toolchain, statically (libsqlite3.a), installed only into the
# build-time dependency prefix (build/deps/sqlite); intentionally NOT
# staged into the rootfs, since it is linked statically into xpkg.
# No GNU runtime dependency.

NAME   := sqlite
VERSION:= 3.45.3
CATEGORY:= base
LICENSE:= Public Domain
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Build-time dependency prefix (xpkg consumes it via SQLITE_PREFIX
# / -lsqlite3).  `=` (recursive): resolved lazily after project-port.mk
# binds FREELINX_BUILD_DIR.
SQLITE_PREFIX = $(FREELINX_BUILD_DIR)/deps/$(NAME)

# Not staged into the rootfs (static archive linked into consumers).
INSTALL_BIN:=
INSTALL_RELPATH:= lib/libsqlite3.a

include ../../mk/project-port.mk

# The FreeLinX toolchain's llvm-ar/llvm-ranlib need GLIBC_2.38 which the
# build host (GLIBC 2.36) does not provide, so they cannot run here.
# Archiving is libc-agnostic, so we use the host ar/ranlib to package the
# musl .o files; the final consumers (xpkg) still link ONLY with FreeLinX
# clang+LLD+musl. (Same pattern as base/zlib and base/openssl.)
FLX_HOST_AR   ?= /usr/bin/ar
FLX_HOST_RANLIB ?= /usr/bin/ranlib

# sqlite ships an autoconf amalgamation; drive it with the exported
# FreeLinX CC/CFLAGS/LDFLAGS (project-port.mk exports them).
PROJECT_CFG_CMDS = \
	AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)" ./configure \
		--disable-shared --enable-static --disable-threadsafe \
		--disable-dynamic-extensions \
		--prefix="$(SQLITE_PREFIX)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install

# The delivered artifact is the static archive (never staged to rootfs).
PROJECT_BIN = $(SQLITE_PREFIX)/lib/libsqlite3.a
BUILD_BIN   = $(PROJECT_BIN)
