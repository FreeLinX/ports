# FreeLinX/ports - graphics/mpv : Free, open-source, and cross-platform media player (0.35.1).
#
# Universal audio and video player with X11 output and sndio audio.

NAME    := mpv
VERSION := 0.35.1
CATEGORY:= graphics
LICENSE := GPLv2
DEPENDENCIES:= ffmpeg libass sndio libX11 libXext libXrandr libXinerama libXss libXpresent
BUILD_DEPENDENCIES:= musl, ffmpeg, libass, sndio, libX11
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= mpv
INSTALL_RELPATH:= usr/bin/mpv

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/mpv-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/build/mpv
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	echo "mpv configured"

BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && ninja

install: $(STAGE_FILE)

install-rootfs: check-install-rootfs $(STAGE_FILE)
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
