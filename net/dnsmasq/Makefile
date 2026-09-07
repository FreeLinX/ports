# FreeLinX/ports - net/dnsmasq : dnsmasq 2.90 (DNS/DHCP server).
#
# Lightweight DNS and DHCP server. Plain Makefile build, no autoconf.
# GPL-2.0 license. Built with many optional features disabled for minimal
# static binary.

NAME    := dnsmasq
VERSION := 2.90
CATEGORY:= net
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dnsmasq
INSTALL_RELPATH:= usr/sbin/dnsmasq

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dnsmasq-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/dnsmasq
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
		COPTS="-DNO_IDN2 -DNO_LIBIDN2 -DNO_NLS -DNO_GETTEXT -DNO_AUTH -DNO_CONNTRACK -DNO_DBUS -DNO_LUA -DNO_UBUS -DNO_TFTP -DNO_DHCP6"
