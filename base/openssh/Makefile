# FreeLinX/ports - base/openssh : OpenSSH 10.5p1 (upstream portable port).
#
# Real upstream OpenSSH (portable) built with the FreeLinX clang + LLD + musl
# toolchain, fully static.  Dependencies:
#   - zlib     (base/zlib, static)
#   - libcrypto/libssl (base/openssl, static, no-shared)
# Disabled: PAM (no libpam available for static musl; key-based auth works
# without it), lastlog, and the optional extras.  The core client/server
# binaries (ssh, sshd, ssh-keygen) plus scp/sftp are produced as genuine
# statically-linked musl binaries with NO GNU runtime dependency.
#
# Note: sshd requires a privilege-separated runtime environment (/var/empty, a
# sshd user, /etc/ssh/sshd_config) and a kernel; that is exercised only on the
# FreeLinX target, not faked here on the build host.

NAME   := openssh
VERSION:= 10.5p1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:= zlib openssl
BUILD_DEPENDENCIES:= musl, zlib (base/zlib), OpenSSL (base/openssl)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ssh
INSTALL_RELPATH:= bin/ssh

# Build-time dependency prefixes produced by base/zlib and base/openssl.
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so these must resolve lazily.
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib
OSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl

# OpenSSH is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

# Archives: llvm-ar/llvm-ranlib from the FreeLinX toolchain (no GNU binutils).
FLX_HOST_AR      ?= $(FREELINX_TOOLCHAIN_BIN)/llvm-ar
FLX_HOST_RANLIB  ?= $(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/ssh
BUILD_BIN   = $(PROJECT_BIN)

# OpenSSH drives its own autoconf configure; point it at the static musl zlib
# and OpenSSL, and disable PAM/lastlog for a minimal static build.
PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--sysconfdir=/etc/ssh \
		--with-ssl-dir="$(OSSL_PREFIX)" \
		--with-zlib="$(ZLIB_PREFIX)" \
		--without-pam \
		--disable-lastlog \
		--with-privsep-path=/var/empty \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -I$(OSSL_PREFIX)/include -I$(ZLIB_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(OSSL_PREFIX)/lib64 -L$(ZLIB_PREFIX)/lib" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
