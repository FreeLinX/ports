# FreeLinX/ports - www/links : Links 2.30 text and graphic web browser.
#
# The QEMU-tested Links web browser (text mode + framebuffer graphics),
# statically linked, shipped in the FreeLinX desktop.
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains.

NAME    := links
VERSION := 2.30
CATEGORY:= www
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= links
INSTALL_RELPATH:= usr/bin/links

include ../../mk/project-port.mk