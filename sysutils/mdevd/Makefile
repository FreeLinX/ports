# FreeLinX/ports - sysutils/mdevd : mdev-compatible uevent daemon 0.1.8.1.
#
# Standalone, netlink-based hotplug device manager for /dev node creation.
# Depends on sysutils/skalibs.

NAME   := mdevd
VERSION:= 0.1.8.1
CATEGORY:= sysutils
LICENSE:= ISC
DEPENDENCIES:= skalibs
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), skalibs (sysutils/skalibs)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= mdevd
INSTALL_RELPATH:= sbin/mdevd

SKALIBS_PREFIX = $(FREELINX_BUILD_DIR)/deps/skalibs

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

PROJECT_BIN = $(SRC_TREE)/mdevd
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--enable-static \
		--disable-shared \
		--with-include="$(SKALIBS_PREFIX)/include" \
		--with-lib="$(SKALIBS_PREFIX)/lib" \
		--with-sysdeps="$(SKALIBS_PREFIX)/lib/skalibs/sysdeps" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(SKALIBS_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(SKALIBS_PREFIX)/lib" \
		AR="$(FLX_HOST_AR)" RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" mdevd mdevd-coldplug

install: $(STAGE_FILE)
	@if [ -f "$(SRC_TREE)/mdevd-coldplug" ]; then \
		printf '[FreeLinX/ports] staging mdevd-coldplug -> %s/sbin/mdevd-coldplug\n' "$(STAGE_ROOT)"; \
		mkdir -p "$(STAGE_ROOT)/sbin"; \
		install -m 755 "$(SRC_TREE)/mdevd-coldplug" "$(STAGE_ROOT)/sbin/mdevd-coldplug"; \
	fi

install-rootfs: check-install-rootfs $(STAGE_FILE)
	@printf '[FreeLinX/ports] installing %s into rootfs %s\n' "$(NAME)" "$(ROOTFS_FILE)"
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@if [ -f "$(STAGE_ROOT)/sbin/mdevd-coldplug" ]; then \
		mkdir -p "$(ROOTFS_DIR)/sbin"; \
		install -m 755 "$(STAGE_ROOT)/sbin/mdevd-coldplug" "$(ROOTFS_DIR)/sbin/mdevd-coldplug"; \
	fi
