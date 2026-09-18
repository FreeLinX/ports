# FreeLinX/ports - x11/openbox : Openbox 3.6.1 window manager.
#
# The QEMU-tested Openbox window manager for the FreeLinX desktop.  Ships the
# xtree binary tree (/usr/share/X11/xtree/bin, where the real openbox binary
# lives and /usr/bin/openbox points at), themes, XDG config, and the
# openbox-autostart helper.
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains (symlinks included).

NAME    := openbox
VERSION := 3.6.1
CATEGORY:= x11
LICENSE := GPL-2.0
DEPENDENCIES:= xorg
BUILD_DEPENDENCIES:= X11 stack (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

PKG_DEPENDS:= xorg

INSTALL_BIN:= openbox
INSTALL_RELPATH:= usr/bin/openbox

include ../../mk/project-port.mk