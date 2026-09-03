# FreeLinX/ports - sysutils/doas : OpenDoas 6.8.2 (BSD doas for Linux).
#
# OpenBSD doas port by Duncaen built with FreeLinX clang + LLD + musl.
# Configured without PAM (shadow/musl native password verification).
# Fully static binar staged to /usr/bin/doas.

NAME   := doas
VERSION:= 6.8.2
CATEGORY:= sysutils
LICENSE:= ISC
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= doas
INSTALL_RELPATH:= usr/bin/doas

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/OpenDoas-6.8.2
PROJECT_BIN = $(SRC_TREE)/doas
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	CC="$(FREELINX_CC)" \
	CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
	LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt" \
	./configure \
		--prefix=/usr \
		--sysconfdir=/etc \
		--without-pam \
		--enable-static

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" doas
