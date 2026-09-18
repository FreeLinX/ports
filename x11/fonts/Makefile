# FreeLinX/ports - x11/fonts : FreeLinX default TTF fonts (DejaVu family).
#
# The QEMU-tested font set for the FreeLinX desktop: the DejaVu TTF family
# plus the fontconfig configuration (fonts.conf).
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains.

NAME    := fonts
VERSION := 1.0
CATEGORY:= x11
LICENSE := Bitstream-Vera
DEPENDENCIES:=
BUILD_DEPENDENCIES:= (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= fonts
INSTALL_RELPATH:= usr/share/fonts/TTF

include ../../mk/project-port.mk