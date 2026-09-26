# FreeLinX/ports - firmware/linux-firmware : redistributable firmware blobs.
#
# linux-firmware (git.kernel.org) is the canonical collection of firmware
# blobs required by WiFi/Ethernet/GPU/etc drivers. It is NOT Free Software but
# is redistributable; this is the documented, unavoidable exception for a
# wireless stack (see ports README.md).
#
# DATA-ONLY port: nothing is compiled. do-build extracts the upstream release
# tarball; install/install-rootfs copy the whole firmware/ tree into the
# FreeLinX rootfs at /lib/firmware so any driver FreeLinX enables (ath9k/
# ath10k/iwlwifi/rtw88/rtw89/brcmfmac + Ethernet/GPU) finds its blobs.
#
# The generic single-binary staging rules in mk/install.mk are neutralised by
# pointing them at a marker file; the real work is the explicit copy recipes
# below (single-colon, so they accumulate with the framework's phony targets).

NAME   := linux-firmware
VERSION:= 20240811
CATEGORY:= firmware
LICENSE:= various (GPL-2.0 / MIT / BSD-clause / proprietary-redistributable)
DEPENDENCIES:=
BUILD_DEPENDENCIES:=
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Neutralise the generic single-binary staging with an inert marker.
INSTALL_BIN:= linux-firmware
INSTALL_RELPATH:= lib/firmware/.flx-firmware-marker

include ../../mk/port.mk

# Nothing in this tree vendors the firmware tarball, and nothing in the build
# downloads it: dist/ holds the inputs for every other port, and this one is
# ~500 MB of redistributable-but-not-Free blobs, which is a different decision
# to make than "check the tarball in".  distinfo records the upstream URL and
# the sha256, so the input is identified precisely; it is simply not here.
#
# So the port declares itself unbuildable rather than failing with tar's
# "Cannot open: No such file or directory", which says nothing about why.  Two
# ways to make it real, both one line each: vendor the tarball into dist/ under
# the name distinfo already gives it, or teach the build to fetch and verify it
# against DISTINFO_SHA256.  Whichever is chosen, deleting this line is the whole
# fix - nothing else in the port changes.
PORT_NOT_PORTABLE:= its ~500 MB upstream tarball is not vendored in ports/dist/ and the build does not download it (see distinfo for the URL and the sha256 it would be verified against)

SRC_DIR = $(FREELINX_BUILD_DIR)/work/$(NAME)/$(DISTINFO_NAME)
DIST_TGZ = $(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)
BUILD_BIN = $(SRC_DIR)/.flx-extracted

# --- extract upstream tarball (idempotent) ---------------------------------
$(BUILD_BIN):
	@set -e; \
	printf '[FreeLinX/ports] extracting %s\n' "$(DISTINFO_ARCHIVE)"; \
	mkdir -p "$(SRC_DIR)"; \
	tar -xf "$(DIST_TGZ)" -C "$(SRC_DIR)"; \
	touch "$(BUILD_BIN)"

do-build: $(BUILD_BIN)
	@printf '[FreeLinX/ports] linux-firmware extracted (stage via install)\n'

# --- stage blobs into the staging overlay lib/firmware ---------------------
install: $(BUILD_BIN)
	@printf '[FreeLinX/ports] staging linux-firmware blobs to staging/lib/firmware\n'
	$(MKDIR) -p "$(STAGE_ROOT)/lib/firmware"
	cp -an "$(SRC_DIR)/." "$(STAGE_ROOT)/lib/firmware/"

# --- copy blobs into the FreeLinX/src rootfs template lib/firmware ---------
install-rootfs: check-install-rootfs install
	@printf '[FreeLinX/ports] installing linux-firmware into %s/lib/firmware\n' "$(ROOTFS_DIR)"
	$(MKDIR) -p "$(ROOTFS_DIR)/lib/firmware"
	cp -an "$(SRC_DIR)/." "$(ROOTFS_DIR)/lib/firmware/"
