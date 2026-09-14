# FreeLinX/ports - graphics/ffmpeg : Universal multimedia converter and streamer (6.1.2).
#
# Complete cross-platform solution to record, convert, and stream audio and video.

NAME    := ffmpeg
VERSION := 6.1.2
CATEGORY:= graphics
LICENSE := GPLv2
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl, nasm
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= ffmpeg
INSTALL_RELPATH:= usr/bin/ffmpeg

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/ffmpeg-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/ffmpeg
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	echo "FFmpeg configured"

BUILD_CMDS = \
	$(MAKE) -C "$(SRC_TREE)" -j"$(FREELINX_JOBS)" ffmpeg

install: $(STAGE_FILE)

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
