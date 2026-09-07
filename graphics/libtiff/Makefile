# FreeLinX/ports - graphics/libtiff : TIFF image library (4.6.0).
#
# Tagged Image File Format library. BSD-like license. CMake build.

NAME    := libtiff
VERSION := 4.6.0
CATEGORY:= graphics
LICENSE := LIBTiff
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/tiff-$(VERSION)
PROJECT_LIB = $(SRC_TREE)/build/libtiff.a

LIBTIFF_CMAKE_FLAGS = \
	-DCMAKE_SYSTEM_NAME=Linux \
	-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
	-DCMAKE_C_COMPILER="$(FREELINX_TOOLCHAIN_DIR)/bin/clang" \
	-DCMAKE_C_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -O2" \
	-DCMAKE_C_LINK_EXECUTABLE="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -fuse-ld=lld --rtlib=compiler-rt <FLAGS> <LINK_FLAGS> <OBJECTS> <LIBRARIES> -o <TARGET>" \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_BUILD_TYPE=Release \
	-Dtiff-tools=OFF \
	-Dtiff-tests=OFF \
	-Dtiff-contrib=OFF \
	-Dtiff-docs=OFF \
	-Djbig=OFF \
	-Dlzma=OFF \
	-Dzstd=OFF \
	-Dwebp=OFF \
	-Dzlib=OFF

PROJECT_CFG_CMDS = \
	mkdir -p "$(SRC_TREE)/build" && \
	cd "$(SRC_TREE)/build" && \
	cmake .. $(LIBTIFF_CMAKE_FLAGS)

BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && \
	make -j"$(FREELINX_JOBS)" tiff

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(STAGE_ROOT)/usr/lib/libtiff.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(SRC_TREE)/libtiff/tiffio.h" "$(STAGE_ROOT)/usr/include/tiffio.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/libtiff/tiff.h" "$(STAGE_ROOT)/usr/include/tiff.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/libtiff/tiffvers.h" "$(STAGE_ROOT)/usr/include/tiffvers.h"
	@printf '[FreeLinX/ports] staged libtiff\n'
