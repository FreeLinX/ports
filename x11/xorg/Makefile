# FreeLinX/ports - x11/xorg : X.Org X Server 21.1.24 (fbdev).
#
# The QEMU-tested FreeLinX X server.  Ships the Xorg binary, xorg.conf, the
# fbdev/evdev driver modules (in both canonical /usr/lib/xorg/modules and the
# xtree ModulePath copies), and the full XKB data tree used by xkbcomp.
#
# This is a rootfs-derived port: the packaged artifacts are staged from the
# FreeLinX/src rootfs template (which is the QEMU-tested source of truth), not
# rebuilt here.  package.sh mirrors the exact staging used to create the
# shipped package.

NAME    := xorg
VERSION := 21.1.24
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= xkbcomp, fonts
BUILD_DEPENDENCIES:= X11 stack (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

PKG_DEPENDS:= xkbcomp,fonts

INSTALL_BIN:= Xorg
INSTALL_RELPATH:= usr/bin/Xorg

include ../../mk/project-port.mk