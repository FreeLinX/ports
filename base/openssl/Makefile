# FreeLinX/ports - base/openssl : OpenSSL 3.3.2 (build dependency, upstream).
#
# Real upstream OpenSSL built with the FreeLinX clang + LLD + musl toolchain,
# statically (libssl.a / libcrypto.a).  It is a build-time dependency for
# OpenSSH and is installed only into the build-time dependency prefix
# (build/deps/openssl); it is intentionally NOT staged into the rootfs, since
# it is linked statically into its consumers.  No GNU runtime dependency.

NAME   := openssl
VERSION:= 3.3.2
CATEGORY:= base
LICENSE:= Apache-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl, perl (host) (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Build-time dependency prefix (openssh consumes it via --with-ssl-dir).
OSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/$(NAME)

# OpenSSL is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

# The FreeLinX toolchain's llvm-ar/llvm-ranlib/llvm-nm require GLIBC_2.38 (host
# has 2.36) and cannot run here; archiving is libc-agnostic so host tools
# package the musl .o files.  The final OpenSSL link uses ONLY FreeLinX
# clang+LLD.
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(OSSL_PREFIX)/lib/libcrypto.a
BUILD_BIN   = $(PROJECT_BIN)

# OpenSSL drives its own Configure; pass the FreeLinX toolchain explicitly.
PROJECT_CFG_CMDS = \
	./Configure linux-x86_64 \
		--prefix="$(OSSL_PREFIX)" \
		--openssldir="$(OSSL_PREFIX)/ssl" \
		no-shared no-tests no-dso no-ui-console no-ssl3 no-comp \
		no-afalgeng no-threads \
		"CC=$(FREELINX_CC)" \
		"CFLAGS=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		"LDFLAGS=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt" \
		"AR=$(FLX_HOST_AR)" "RANLIB=$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install_sw install_ssldirs
