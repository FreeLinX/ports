# FreeLinX/ports - graphics/libpng : Portable Network Graphics library (1.6.43).
#
# PNG image library. Required by NetSurf.
# libpng license (libpng-2.0). CMake build.
# Requires: zlib.

NAME    := libpng
VERSION := 1.6.43
CATEGORY:= graphics
LICENSE := libpng
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libpng-$(VERSION)
PROJECT_LIB = $(SRC_TREE)/build/libpng16.a

ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

LIBPNG_CMAKE_FLAGS = \
	-DCMAKE_SYSTEM_NAME=Linux \
	-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
	-DCMAKE_C_COMPILER="$(FREELINX_TOOLCHAIN_DIR)/bin/clang" \
	-DCMAKE_C_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -O2" \
	-DCMAKE_C_LINK_EXECUTABLE="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -fuse-ld=lld --rtlib=compiler-rt <FLAGS> <LINK_FLAGS> <OBJECTS> <LIBRARIES> -o <TARGET>" \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_BUILD_TYPE=Release \
	-DPNG_SHARED=OFF \
	-DPNG_TESTS=OFF \
	-DZLIB_ROOT="$(ZLIB_PREFIX)"

PROJECT_CFG_CMDS = \
	mkdir -p "$(SRC_TREE)/build" && \
	cd "$(SRC_TREE)/build" && \
	cmake .. $(LIBPNG_CMAKE_FLAGS)

BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && \
	make -j"$(FREELINX_JOBS)" png_static

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(STAGE_ROOT)/usr/lib/libpng16.a"
	@$(INSTALL) -m 644 "$(SRC_TREE)/png.h" "$(STAGE_ROOT)/usr/include/png.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/pngconf.h" "$(STAGE_ROOT)/usr/include/pngconf.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/build/pnglibconf.h" "$(STAGE_ROOT)/usr/include/pnglibconf.h"
	@printf '[FreeLinX/ports] staged libpng\n'
