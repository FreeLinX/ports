# FreeLinX/ports - devel/freetype2 : FreeType 2 font library (2.13.2).
#
# TrueType/OpenType font rasterizer. Required by NetSurf.
# FreeType FTL/BSD-style license. CMake build.

NAME    := freetype2
VERSION := 2.13.2
CATEGORY:= devel
LICENSE := FTL
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/freetype-VER-$(shell echo $(VERSION) | tr '.' '-')
PROJECT_LIB = $(SRC_TREE)/build/libfreetype.a

FREETYPE_CMAKE_FLAGS = \
	-DCMAKE_SYSTEM_NAME=Linux \
	-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
	-DCMAKE_C_COMPILER="$(FREELINX_TOOLCHAIN_DIR)/bin/clang" \
	-DCMAKE_C_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -O2" \
	-DCMAKE_C_LINK_EXECUTABLE="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -fuse-ld=lld --rtlib=compiler-rt <FLAGS> <LINK_FLAGS> <OBJECTS> <LIBRARIES> -o <TARGET>" \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_BUILD_TYPE=Release \
	-DFT_DISABLE_BZIP2=ON \
	-DFT_DISABLE_BROTLI=ON \
	-DFT_DISABLE_HARFBUZZ=ON \
	-DFT_DISABLE_PNG=ON

PROJECT_CFG_CMDS = \
	mkdir -p "$(SRC_TREE)/build" && \
	cd "$(SRC_TREE)/build" && \
	cmake .. $(FREETYPE_CMAKE_FLAGS)

BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && \
	make -j"$(FREELINX_JOBS)" freetype

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(STAGE_ROOT)/usr/lib/libfreetype.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/freetype"
	@cp -r "$(SRC_TREE)/include/freetype/." "$(STAGE_ROOT)/usr/include/freetype/"
	@$(INSTALL) -m 644 "$(SRC_TREE)/include/ft2build.h" "$(STAGE_ROOT)/usr/include/ft2build.h"
	@printf '[FreeLinX/ports] staged freetype2\n'
