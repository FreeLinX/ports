# FreeLinX/ports - graphics/mupdf : Lightweight PDF, XPS, and EPUB viewer (1.24.10).
#
# Ultra-fast, lightweight document reader with X11 frontend.

NAME    := mupdf
VERSION := 1.24.10
CATEGORY:= graphics
LICENSE := AGPL-3.0-or-later
DEPENDENCIES:= freetype harfbuzz libjpeg libpng zlib libX11
BUILD_DEPENDENCIES:= musl, freetype, harfbuzz, libX11
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= mupdf
INSTALL_RELPATH:= usr/bin/mupdf

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/mupdf-$(VERSION)-source
PROJECT_BIN = $(SRC_TREE)/build/release/mupdf-x11
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	echo "MuPDF configured"

BUILD_CMDS = \
	$(MAKE) -C "$(SRC_TREE)" -j"$(FREELINX_JOBS)" build/release/mupdf-x11

install: $(STAGE_FILE)

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	ln -sf mupdf $(dir $(ROOTFS_FILE))/mupdf-x11
