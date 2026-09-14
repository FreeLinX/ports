# FreeLinX/ports - x11/setxkbmap : Set the keyboard using the X Keyboard Extension (1.3.5).
#
# Configure keyboard layouts, variants, and toggle options (e.g. AZ/US toggle).

NAME    := setxkbmap
VERSION := 1.3.5
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libxkbfile libXrandr libX11
BUILD_DEPENDENCIES:= musl, libxkbfile, libX11
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= setxkbmap
INSTALL_RELPATH:= usr/bin/setxkbmap

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/setxkbmap-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/setxkbmap
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure --host=$(FREELINX_TRIPLE) --prefix=/usr \
		--with-xkb-config-root=/usr/share/X11/xkb \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -unwindlib=none" \
		SETXKBMAP_CFLAGS="-I$(FREELINX_PORTS_ROOT)/../desktop-test/ports/build/deps/x11/include" \
		SETXKBMAP_LIBS="-L$(FREELINX_PORTS_ROOT)/../desktop-test/ports/build/deps/x11/lib -lxkbfile -lXrandr -lXrender -lXext -lX11 -lxcb -lXau -lXdmcp -lm"

BUILD_CMDS = \
	$(MAKE) -C "$(SRC_TREE)" -j"$(FREELINX_JOBS)"

install: $(STAGE_FILE)

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
