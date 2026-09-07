# FreeLinX/ports - net/iproute2 : iproute2 6.9.0 (network configuration).
#
# Advanced network configuration tools (ip, bridge, tc, ss).
# GPL-2.0 license. Raw Makefile with configure script (not autoconf).
# Multi-binary port: stages ip, bridge, ss, tc.
# configure writes config.mk; optional deps (libmnl, libelf) are skipped
# gracefully by the configure probe when not present in the sysroot.

NAME    := iproute2
VERSION := 6.9.0
CATEGORY:= net
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= ip
INSTALL_RELPATH:= usr/sbin/ip

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/iproute2-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/ip/ip
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	CC="$(FREELINX_CC)" \
	AR="$(FREELINX_AR)" \
	PKG_CONFIG=false \
	./configure --include_dir="$(SRC_TREE)/include"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		AR="$(FREELINX_AR)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -DHAVE_SETNS -DHAVE_HANDLE_AT -DNEED_STRLCPY -DRESOLVE_HOSTNAMES -DLIBDIR=\\\"/usr/lib\\\" -DNO_SHARED_LIBS -DCONF_USR_DIR=\\\"/usr/share/iproute2\\\" -DCONF_ETC_DIR=\\\"/etc/iproute2\\\" -DNETNS_RUN_DIR=\\\"/var/run/netns\\\" -DNETNS_ETC_DIR=\\\"/etc/netns\\\" -DARPDDIR=\\\"/var/lib/arpd\\\" -DCONF_COLOR=COLOR_OPT_NEVER -I$(SRC_TREE)/include -I$(SRC_TREE)/include/uapi" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
		KERNEL_INCLUDE="$(FREELINX_SYSROOT)/usr/include" \
		SHARED_LIBS=n \
		HAVE_BERKELEY_DB=n

# ---------------------------------------------------------------------------
# Extra staged binaries beyond INSTALL_BIN/INSTALL_RELPATH.
# iproute2 builds ip, bridge, ss, tc (and others we don't stage).
# Follows the runit multi-binary pattern from sysutils/runit.
# ---------------------------------------------------------------------------
install: $(STAGE_ROOT)/usr/sbin/bridge $(STAGE_ROOT)/usr/sbin/ss $(STAGE_ROOT)/usr/sbin/tc

$(STAGE_ROOT)/usr/sbin/bridge: $(SRC_TREE)/bridge/bridge
	@printf '[FreeLinX/ports] staging bridge -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@

$(STAGE_ROOT)/usr/sbin/ss: $(SRC_TREE)/misc/ss
	@printf '[FreeLinX/ports] staging ss -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@

$(STAGE_ROOT)/usr/sbin/tc: $(SRC_TREE)/tc/tc
	@printf '[FreeLinX/ports] staging tc -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@
