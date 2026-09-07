# FreeLinX/ports - devel/pkgconf : pkg-config replacement (2.3.0).
#
# pkgconf is the lightweight, ISC-licensed drop-in replacement for
# pkg-config, resolving .pc dependency metadata. There is no GNU tool in
# the system or the build. Configure path (autoconf-style, non-GNU).

NAME    := pkgconf
VERSION := 2.3.0
CATEGORY:= devel
LICENSE := ISC
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

# pkgconf is a host/build tool used at build time, but FreeLinX stages it
# into the rootfs too so developers can resolve .pc files in the live system.
INSTALL_BIN:= pkgconf
INSTALL_RELPATH:= usr/bin/pkgconf

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/pkgconf-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/pkgconf
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--enable-static \
		--disable-shared \
		--with-pkg-config-dir=/usr/lib/pkgconfig:/usr/share/pkgconfig \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
