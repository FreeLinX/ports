# FreeLinX/ports - x11/xcalc : xcalc 1.1.2 (X11 scientific calculator).
#
# Classic scientific calculator for the X Window System.
# 100% non-GNU, static Musl libc + Clang build.

NAME    := xcalc
VERSION := 1.1.2
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= xcalc
INSTALL_RELPATH:= usr/bin/xcalc

include ../../mk/project-port.mk

PROJECT_BIN = $(FREELINX_BUILD_DIR)/deps/$(NAME)/usr/bin/xcalc
BUILD_BIN   = $(PROJECT_BIN)

BUILD_CMDS = \
	echo "xcalc already built and staged in $(PROJECT_BIN)"
