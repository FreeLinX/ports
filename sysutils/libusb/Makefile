# FreeLinX/ports - sysutils/libusb : libusb 1.0.27 (USB access library).
#
# Library for generic USB access. Build dependency for usbutils.

NAME    := libusb
VERSION := 1.0.27
CATEGORY:= sysutils
LICENSE := LGPL-2.1
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= lib/libusb-1.0.a

LIBUSB_PREFIX = $(FREELINX_BUILD_DIR)/deps/libusb

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libusb-$(VERSION)
PROJECT_BIN = $(LIBUSB_PREFIX)/lib/libusb-1.0.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix="$(LIBUSB_PREFIX)" \
		--enable-static \
		--disable-shared \
		--disable-udev \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install
