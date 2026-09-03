# FreeLinX/ports - sysutils/skalibs : skarnet C library 2.14.3.0.
#
# Build dependency for mdevd. Built as static library libskalibs.a into
# build/deps/skalibs prefix.

NAME   := skalibs
VERSION:= 2.14.3.0
CATEGORY:= sysutils
LICENSE:= ISC
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

SKALIBS_PREFIX = $(FREELINX_BUILD_DIR)/deps/$(NAME)

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SKALIBS_PREFIX)/lib/libskalibs.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix="$(SKALIBS_PREFIX)" \
		--enable-static \
		--disable-shared \
		--with-sysdeps="$(FREELINX_SYSROOT)/usr/lib" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install
