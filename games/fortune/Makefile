# FreeLinX/ports - games/fortune : NetBSD fortune 10.1 (random quotes generator).
#
# Classic BSD fortune cookies utility with fortunes databases.
# 100% non-GNU, static Musl libc + Clang build.

NAME    := fortune
VERSION := 10.1
CATEGORY:= games
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= fortune
INSTALL_RELPATH:= usr/bin/fortune

include ../../mk/project-port.mk

PROJECT_BIN = $(FREELINX_BUILD_DIR)/deps/$(NAME)/usr/bin/fortune
BUILD_BIN   = $(PROJECT_BIN)

BUILD_CMDS = \
	echo "fortune already built and staged in $(PROJECT_BIN)"
