# FreeLinX/ports - sysutils/socklog : socklog 2.2.3 (logging for runit).
#
# socklog is a syslog implementation using Unix domain sockets, designed
# to work with runit's service supervision. It receives log messages on
# /dev/log and stores them in runit-style log directories.
#
# This is the just-containers/socklog fork that uses skalibs (already
# ported in sysutils/skalibs).

NAME    := socklog
VERSION := 2.2.3
CATEGORY:= sysutils
LICENSE := BSD-3-Clause
DEPENDENCIES:= skalibs
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), skalibs (sysutils/skalibs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= socklog
INSTALL_RELPATH:= sbin/socklog

SKALIBS_PREFIX = $(FREELINX_BUILD_DIR)/deps/skalibs

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =
FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/socklog-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/socklog
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
	make -j"$(FREELINX_JOBS)"

# socklog produces 3 binaries: socklog, tryto, uncat
# Stage all three
install: $(STAGE_FILE)
	@if [ -f "$(SRC_TREE)/tryto" ]; then \
		mkdir -p "$(STAGE_ROOT)/usr/bin"; \
		install -m 755 "$(SRC_TREE)/tryto" "$(STAGE_ROOT)/usr/bin/tryto"; \
	fi
	@if [ -f "$(SRC_TREE)/uncat" ]; then \
		mkdir -p "$(STAGE_ROOT)/usr/bin"; \
		install -m 755 "$(SRC_TREE)/uncat" "$(STAGE_ROOT)/usr/bin/uncat"; \
	fi

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@if [ -f "$(SRC_TREE)/tryto" ]; then \
		mkdir -p "$(ROOTFS_DIR)/usr/bin"; \
		install -m 755 "$(SRC_TREE)/tryto" "$(ROOTFS_DIR)/usr/bin/tryto"; \
	fi
	@if [ -f "$(SRC_TREE)/uncat" ]; then \
		mkdir -p "$(ROOTFS_DIR)/usr/bin"; \
		install -m 755 "$(SRC_TREE)/uncat" "$(ROOTFS_DIR)/usr/bin/uncat"; \
	fi
