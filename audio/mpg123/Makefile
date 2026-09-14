# FreeLinX/ports - audio/mpg123 : Fast console MPEG audio player (1.32.10)
#
# mpg123 is the standard real-time audio player, built statically against musl
# and tinyalsa with full x86-64 SIMD optimizations.

NAME    := mpg123
VERSION := 1.32.10
CATEGORY:= audio
LICENSE := LGPL-2.1-only
DEPENDENCIES:= tinyalsa
BUILD_DEPENDENCIES:= musl, tinyalsa
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= mpg123
INSTALL_RELPATH:= usr/bin/mpg123

TINY_INC:= $(FREELINX_STAGING_DIR)/usr/include
TINY_LIB:= $(FREELINX_STAGING_DIR)/usr/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/mpg123-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/mpg123
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--enable-static \
		--disable-shared \
		--with-audio=tinyalsa \
		--with-default-audio=tinyalsa \
		--with-cpu=x86-64 \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(TINY_INC)" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -unwindlib=none -L$(TINY_LIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" src/mpg123

install: $(STAGE_FILE)

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
