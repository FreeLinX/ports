# FreeLinX/ports - devel/tcc : Tiny C Compiler (0.9.27)
# Self-hosting C compiler for FreeLinX.

NAME    := tcc
VERSION := 0.9.27
CATEGORY:= devel
LICENSE := LGPL-2.1-only
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= tcc
INSTALL_RELPATH:= usr/bin/tcc

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/tcc-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/tcc
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--bindir=/usr/bin \
		--libdir=/usr/lib \
		--includedir=/usr/include \
		--tccdir=/usr/lib/tcc \
		--config-musl \
		--enable-static \
		--triplet=$(FREELINX_TRIPLE) \
		--cc="$(FREELINX_CC)" \
		--ar="ar" \
		--extra-cflags="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -DTCC_MUSL=1" \
		--extra-ldflags="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -unwindlib=none"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" tcc libtcc1.a

install: $(STAGE_FILE)
	@mkdir -p $(STAGE_ROOT)/usr/bin $(STAGE_ROOT)/usr/lib/tcc/include $(STAGE_ROOT)/bin
	@ln -sf tcc $(STAGE_ROOT)/usr/bin/cc
	@ln -sf tcc $(STAGE_ROOT)/usr/bin/gcc
	@ln -sf ../usr/bin/tcc $(STAGE_ROOT)/bin/cc
	@cp -f $(SRC_TREE)/libtcc1.a $(STAGE_ROOT)/usr/lib/tcc/
	@cp -f $(SRC_TREE)/include/*.h $(STAGE_ROOT)/usr/lib/tcc/include/

install-rootfs: check-install-rootfs $(STAGE_FILE)
	@mkdir -p $(ROOTFS_DIR)/usr/bin $(ROOTFS_DIR)/usr/lib/tcc/include $(ROOTFS_DIR)/bin
	@install -m 755 $(STAGE_FILE) $(ROOTFS_DIR)/usr/bin/tcc
	@ln -sf tcc $(ROOTFS_DIR)/usr/bin/cc
	@ln -sf tcc $(ROOTFS_DIR)/usr/bin/gcc
	@ln -sf ../usr/bin/tcc $(ROOTFS_DIR)/bin/cc
	@cp -f $(SRC_TREE)/libtcc1.a $(ROOTFS_DIR)/usr/lib/tcc/
	@cp -f $(SRC_TREE)/include/*.h $(ROOTFS_DIR)/usr/lib/tcc/include/
