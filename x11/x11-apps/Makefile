# FreeLinX/ports - x11/x11-apps : X11 toy programs (xeyes, xmag, xdemo, xsetroot, glxgears).
#
# The QEMU-tested X11 demonstration programs shipped in the FreeLinX desktop.
# Used by the Openbox menu (glxgears, flx3dtest) for quick 3D sanity checks.
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains.

NAME    := x11-apps
VERSION := 1.0
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= xorg
BUILD_DEPENDENCIES:= X11 stack (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= xeyes
INSTALL_RELPATH:= usr/bin/xeyes

include ../../mk/project-port.mk