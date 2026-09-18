# FreeLinX/ports - x11/urxvt : rxvt-unicode (urxvt) terminal emulator.
#
# The QEMU-tested X11 terminal emulator for the FreeLinX desktop.  Ships
# /usr/bin/urxvt plus the rxvt -> urxvt compatibility symlink.
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains (symlinks included).

NAME    := urxvt
VERSION := 9.31
CATEGORY:= x11
LICENSE := GPL-2.0
DEPENDENCIES:= xorg, fonts
BUILD_DEPENDENCIES:= X11 stack (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

PKG_DEPENDS:= xorg,fonts

INSTALL_BIN:= urxvt
INSTALL_RELPATH:= usr/bin/urxvt

include ../../mk/project-port.mk