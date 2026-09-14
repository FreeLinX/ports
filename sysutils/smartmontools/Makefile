# FreeLinX/ports - sysutils/smartmontools : S.M.A.R.T. monitoring utilities (7.4)
#
# smartctl monitors and controls self-monitoring, analysis and reporting
# technology for hard disks, solid-state drives and NVMe storage.

NAME    := smartmontools
VERSION := 7.4
CATEGORY:= sysutils
LICENSE := GPL-2.0-or-later
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl, libc++
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= smartctl
INSTALL_RELPATH:= usr/sbin/smartctl

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/smartmontools-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/smartctl
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--sbindir=/usr/sbin \
		--without-systemd \
		--without-libcap-ng \
		CC="$(FREELINX_CC)" \
		CXX="$(FREELINX_CXX)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		CXXFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -stdlib=libc++" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -unwindlib=none -L$(FREELINX_TOOLCHAIN_DIR)/lib/$(FREELINX_TRIPLE) -lc++ -lc++abi -lunwind"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" smartctl

install: $(STAGE_FILE)

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
