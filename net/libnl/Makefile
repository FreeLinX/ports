# FreeLinX/ports - net/libnl : libnl 3.9.0 (netlink socket library).
#
# Upstream libnl-3.x built statically with the FreeLinX clang + LLD + musl
# toolchain. This is a build/runtime dependency for wpa_supplicant (it links
# libnl-genl for cfg80211 netlink). License: LGPL-2.1-or-later.
#
# Like base/openssl, this is a STATIC LIBRARY dependency: it is built and
# installed into the build-time dependency prefix (build/deps/libnl) and is
# linked statically into its consumer (wpa_supplicant). It is intentionally
# NOT staged into the rootfs on its own.
#
# The release tarball ships a prebuilt ./configure (autotools), so no
# autogen.sh is needed and the host needs no autotools installed.

NAME   := libnl
VERSION:= 3.9.0
CATEGORY:= net
LICENSE:= LGPL-2.1-or-later
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Build-time dependency prefix (wpa_supplicant consumes via its include/lib dirs).
LIBnl_PREFIX = $(FREELINX_BUILD_DIR)/deps/libnl

# libnl is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

# LLVM's llvm-ar/ranlib need GLIBC_2.38 (host has 2.36); archiving is
# libc-agnostic, so host tools package the musl .o files (as openssl does).
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

# The file libnl always produces; exist-gate for the framework. wpa_supplicant
# links the static libs from build/deps/libnl/lib.
PROJECT_BIN = $(LIBnl_PREFIX)/lib/libnl-3.a
BUILD_BIN   = $(PROJECT_BIN)

# autotools configure driven with the FreeLinX toolchain, installed into the
# deps prefix. Static-only (--disable-shared) since consumers link statically.
PROJECT_CFG_CMDS = \
	./configure \
		--prefix="$(LIBnl_PREFIX)" \
		--host=x86_64-unknown-linux-musl \
		--disable-shared --enable-static \
		CC="$(FREELINX_CC)" \
		AR="$(FLX_HOST_AR)" \
		RANLIB="$(FLX_HOST_RANLIB)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install
