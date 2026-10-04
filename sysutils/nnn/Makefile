# FreeLinX/ports - sysutils/nnn : nnn 5.0 (fast terminal file manager).
#
# Tiny, lightning fast, full-featured terminal file manager.
# 100% non-GNU, static Musl libc + Clang build.

NAME    := nnn
VERSION := 5.0
CATEGORY:= sysutils
LICENSE := BSD-2-Clause
DEPENDENCIES:= netbsd-curses
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), netbsd-curses
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= nnn
INSTALL_RELPATH:= usr/bin/nnn

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/nnn-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/nnn
BUILD_BIN   = $(PROJECT_BIN)

BUILD_CMDS = \
