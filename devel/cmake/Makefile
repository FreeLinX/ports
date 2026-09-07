# FreeLinX/ports - devel/cmake : CMake 3.30.2 (cross-platform build system).
#
# CMake is an open-source, cross-platform family of tools designed to build,
# test and package software. BSD-3-Clause licensed.
#
# Build model: CMake bootstraps itself via its own ./bootstrap script. The
# bootstrap step requires a working host compiler FIRST (it builds a bootstrap
# cmake as a host binary), after which the real cmake is compiled and linked
# against the FreeLinX clang + LLD + musl toolchain using the same bootstrap
# binary. Because cmake's bootstrap cannot run under the FreeLinX toolchain
# alone, this port documents that ordering as a build dependency note.
#
# Compute-heavy: builds every enabled module. Populated by do-build.

NAME    := cmake
VERSION := 3.30.2
CATEGORY:= devel
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= host gcc (bootstrap), then FreeLinX toolchain (clang+LLD+musl)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= cmake
INSTALL_RELPATH:= usr/bin/cmake

# Build-time dependency prefix for the staged cmake installation.
CMAKE_PREFIX = $(FREELINX_BUILD_DIR)/deps/cmake

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/cmake-$(VERSION)
PROJECT_BIN = $(CMAKE_PREFIX)/bin/cmake
BUILD_BIN   = $(PROJECT_BIN)

# Bootstrap creates a host cmake first, then compiles the real one against the
# FreeLinX toolchain (exported CC/CXX/CFLAGS/LDFLAGS by project-port.mk).
# --no-qt-gui avoids the Qt dependency; OpenSSL support is disabled.
PROJECT_CFG_CMDS = \
	./bootstrap \
		--prefix=$(CMAKE_PREFIX) \
		--parallel=$(FREELINX_JOBS) \
		-- \
		-DCMAKE_USE_OPENSSL=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		--no-qt-gui

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
