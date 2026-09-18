# FreeLinX/ports - x11/flxbar-bottom : FreeLinX bottom status bar (i3status → lemonbar).
#
# FreeLinX's desktop status bar: pipes i3status output into lemonbar, pinned
# to the bottom of the screen on the Openbox desktop.
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains.

NAME    := flxbar-bottom
VERSION := 1.0
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= i3status, lemonbar, xrandr
BUILD_DEPENDENCIES:= (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

PKG_DEPENDS:= i3status,lemonbar,xrandr

INSTALL_BIN:= flxbar-bottom
INSTALL_RELPATH:= usr/bin/flxbar-bottom

include ../../mk/project-port.mk