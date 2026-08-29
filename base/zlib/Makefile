# FreeLinX/ports - base/zlib : zlib 1.3.1 (build dependency, upstream port).
#
# Real upstream zlib built with the FreeLinX clang + LLD + musl toolchain,
# statically (libz.a).  It is a build-time dependency for Git and OpenSSH and
# is installed only into the build-time dependency prefix
# (build/deps/zlib); it is intentionally NOT staged into the rootfs, since it
# is linked statically into its consumers.
#
# No GNU runtime dependency: single C library, musl, fully static archive.

NAME   := zlib
VERSION:= 1.3.1
CATEGORY:= base
LICENSE:= Zlib
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Build-time dependency prefix (git/openssh consume it through ZLIB_PATH).
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so this must resolve lazily, not at Makefile-parse time.
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/$(NAME)

# Not staged into the rootfs (it is a static archive linked into consumers).
INSTALL_BIN:=
INSTALL_RELPATH:= lib/libz.a

include ../../mk/project-port.mk

# zlib ships its own configure; drive it with the exported FreeLinX CC/CFLAGS.
# NOTE: the FreeLinX toolchain's llvm-ar/llvm-ranlib require GLIBC_2.38 which
# the build host (GLIBC 2.36) does not provide, so they cannot run here.  Archiving
# is libc-agnostic, so we use the host ar/ranlib to package the musl .o files;
# the final consumers (git, openssh) still link ONLY with FreeLinX clang+LLD+musl.
FLX_HOST_AR   ?= /usr/bin/ar
FLX_HOST_RANLIB ?= /usr/bin/ranlib

PROJECT_CFG_CMDS = \
	AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)" ./configure --static --prefix="$(ZLIB_PREFIX)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" libz.a && \
	make install prefix="$(ZLIB_PREFIX)"

# The delivered artifact is the static archive (never staged to rootfs).
PROJECT_BIN = $(ZLIB_PREFIX)/lib/libz.a
BUILD_BIN   = $(PROJECT_BIN)
