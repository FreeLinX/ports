# FreeLinX/ports - graphics/libjpeg-turbo : JPEG image codec (3.0.3).
#
# libjpeg API-compatible SIMD-accelerated JPEG codec. Required by NetSurf
# and other imaging consumers. BSD-3-Clause style license. CMake build.
# No external static deps; optionally zlib (disabled for a minimal static).

NAME    := libjpeg-turbo
VERSION := 3.0.3
CATEGORY:= graphics
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libjpeg-turbo-$(VERSION)
PROJECT_LIB = $(SRC_TREE)/build/libjpeg.a

LIBJPEG_CMAKE_FLAGS = \
	-DCMAKE_SYSTEM_NAME=Linux \
	-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
	-DCMAKE_C_COMPILER="$(FREELINX_TOOLCHAIN_DIR)/bin/clang" \
	-DCMAKE_C_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -O2" \
	-DCMAKE_C_LINK_EXECUTABLE="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -fuse-ld=lld --rtlib=compiler-rt <FLAGS> <LINK_FLAGS> <OBJECTS> <LIBRARIES> -o <TARGET>" \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_BUILD_TYPE=Release \
	-DENABLE_SHARED=OFF \
	-DENABLE_STATIC=ON \
	-DWITH_TURBOJPEG=OFF \
	-DWITH_JAVA=OFF \
	-DWITH_12BIT=OFF \
	-DWITH_SIMD=OFF

PROJECT_CFG_CMDS = \
	mkdir -p "$(SRC_TREE)/build" && \
	cd "$(SRC_TREE)/build" && \
	cmake .. $(LIBJPEG_CMAKE_FLAGS)

BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && \
	make -j"$(FREELINX_JOBS)" jpeg-static

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(STAGE_ROOT)/usr/lib/libjpeg.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(SRC_TREE)/jpeglib.h" "$(STAGE_ROOT)/usr/include/jpeglib.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/jconfig.h" "$(STAGE_ROOT)/usr/include/jconfig.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/jerror.h" "$(STAGE_ROOT)/usr/include/jerror.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/jmorecfg.h" "$(STAGE_ROOT)/usr/include/jmorecfg.h"
	@printf '[FreeLinX/ports] staged libjpeg-turbo\n'
