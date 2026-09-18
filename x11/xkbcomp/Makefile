# FreeLinX/ports - x11/xkbcomp : X keyboard compiler.
#
# The QEMU-tested XKB compilation tool for the FreeLinX desktop.  Compiles
# keymap descriptions from /usr/share/X11/xkb into X server keymaps.
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains.

NAME    := xkbcomp
VERSION := 1.4.7
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= xorg
BUILD_DEPENDENCIES:= X11 stack (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= xkbcomp
INSTALL_RELPATH:= usr/bin/xkbcomp

include ../../mk/project-port.mk