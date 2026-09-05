# FreeLinX/ports - sysutils/usbutils : usbutils 018 (USB utilities).
#
# Provides lsusb for USB bus device identification.
# Depends on libusb (build dependency).
# Uses meson build system.

NAME    := usbutils
VERSION := 018
CATEGORY:= sysutils
LICENSE := GPL-2.0
DEPENDENCIES:= libusb
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), libusb (sysutils/libusb)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= lsusb
INSTALL_RELPATH:= usr/bin/lsusb

LIBUSB_PREFIX = $(FREELINX_BUILD_DIR)/deps/libusb

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/usbutils-$(VERSION)
BUILD_DIR = $(FREELINX_BUILD_DIR)/work/$(NAME)/build
PROJECT_BIN = $(BUILD_DIR)/lsusb
BUILD_BIN   = $(PROJECT_BIN)

MESON_CROSS = $(FREELINX_BUILD_DIR)/work/$(NAME)/cross.ini

PROJECT_CFG_CMDS = \
	mkdir -p "$(BUILD_DIR)" && \
	if [ ! -f "$(MESON_CROSS)" ]; then \
		printf '[bin]\nclang = "%s"\nld = "%s"\nar = "%s"\n\n[host_machine]\nsystem = "linux"\ncpu_family = "x86_64"\n\n[built-in options]\nc_args = ["-O2", "-I%s/include/libusb-1.0"]\nc_link_args = ["-static", "-fuse-ld=lld", "--rtlib=compiler-rt", "-L%s/lib"]\nprefix = "/usr"\n' "$(FREELINX_CC)" "$(FREELINX_TOOLCHAIN_BIN)/ld.lld" "$(FREELINX_TOOLCHAIN_BIN)/llvm-ar" "$(LIBUSB_PREFIX)" "$(LIBUSB_PREFIX)" > "$(MESON_CROSS)"; \
	fi && \
	meson setup "$(BUILD_DIR)" "$(SRC_TREE)" \
		--cross-file "$(MESON_CROSS)" \
		--default-library=static \
		-Dusbhid=false

BUILD_CMDS = \
	ninja -C "$(BUILD_DIR)" -j"$(FREELINX_JOBS)"
