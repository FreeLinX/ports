# FreeLinX/ports - x11/i3status : i3status 2.8 system status generator.
#
# The QEMU-tested status information generator for the FreeLinX desktop bar:
# outputs CPU/RAM/disk/temp/network stats consumed by flxbar-bottom and
# lemonbar.  Ships the binary plus /etc/i3status.conf.
#
# Rootfs-derived port; package.sh stages exactly what the shipped package
# contains.

NAME    := i3status
VERSION := 2.8
CATEGORY:= x11
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= (prebuilt in rootfs)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= i3status
INSTALL_RELPATH:= usr/bin/i3status

include ../../mk/project-port.mk