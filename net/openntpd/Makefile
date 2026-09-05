# FreeLinX/ports - net/openntpd : OpenNTPD 7.9p1 (NTP time sync daemon).
#
# OpenNTPD is an OpenBSD NTP implementation. The portable version supports
# Linux and builds with musl. We disable HTTPS constraint validation to
# avoid the LibreSSL dependency — the daemon still syncs time via NTP.

NAME    := openntpd
VERSION := 7.9p1
CATEGORY:= net
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= ntpd
INSTALL_RELPATH:= sbin/ntpd

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/openntpd-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/ntpd
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/ \
		--sbindir=/sbin \
		--enable-static \
		--disable-shared \
		--disable-https-constraint \
		--with-privsep-user=root \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# ntpctl is a symlink to ntpd in the install hook
install: $(STAGE_FILE)
	@ln -sf ntpd "$(STAGE_ROOT)/sbin/ntpctl"

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@ln -sf ntpd "$(ROOTFS_DIR)/sbin/ntpctl"
