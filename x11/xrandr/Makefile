# FreeLinX/ports - x11/xrandr : Primitive command line interface to RandR extension (1.5.4).
#
# Control screen resolution, rotation, and multi-monitor configurations in X11.

NAME    := xrandr
VERSION := 1.5.4
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libXrandr libXrender libXext libX11
BUILD_DEPENDENCIES:= musl, libXrandr, libX11
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= xrandr
INSTALL_RELPATH:= usr/bin/xrandr

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/xrandr-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/xrandr
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure --host=$(FREELINX_TRIPLE) --prefix=/usr \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -unwindlib=none" \
		XRANDR_CFLAGS="-I$(FREELINX_PORTS_ROOT)/../desktop-test/ports/build/deps/x11/include" \
		XRANDR_LIBS="-L$(FREELINX_PORTS_ROOT)/../desktop-test/ports/build/deps/x11/lib -lXrandr -lXrender -lXext -lX11 -lxcb -lXau -lXdmcp -lm"

BUILD_CMDS = \
	$(MAKE) -C "$(SRC_TREE)" -j"$(FREELINX_JOBS)"

install: $(STAGE_FILE)

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
