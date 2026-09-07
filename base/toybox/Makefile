# FreeLinX/ports - base/toybox : Toybox 0.8.12 multicall Linux userland.
#
# Toybox is an upstream Linux-native, 0BSD-licensed implementation of small
# system utilities.  It supplies the commands that cannot be directly ported
# from NetBSD because they must use Linux kernel interfaces.
NAME   := toybox
VERSION:= 0.8.12
CATEGORY:= base
LICENSE:= 0BSD
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), make (host)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= toybox
INSTALL_RELPATH:= bin/toybox

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/toybox-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/toybox

# Start from the upstream configuration, then enable the Linux system tools
# FreeLinX needs.  The compiler and linker environment comes exclusively from
# project-port.mk's FreeLinX flags.
PROJECT_CFG_CMDS = sh $(FREELINX_PORTS_ROOT)/base/toybox/config.sh
BUILD_CMDS = make -j"$(FREELINX_JOBS)" toybox
