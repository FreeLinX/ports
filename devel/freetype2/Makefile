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

# LIBRARY_PORT: freetype ships no binary, only libfreetype.a and headers, so
# the framework's staging rules are skipped and the install: target below does
# it.  Declared before the include, because make reads prerequisite lists at
# parse time -- see the STAGE_DEP note in mk/install.mk.
LIBRARY_PORT := yes

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/freetype-VER-$(shell echo $(VERSION) | tr '.' '-')
PROJECT_LIB = $(SRC_TREE)/build/libfreetype.a

# zlib, as built by base/zlib into the build/deps overlay.
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib

FREETYPE_CMAKE_FLAGS = \
	-DCMAKE_SYSTEM_NAME=Linux \
	-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
	-DCMAKE_C_COMPILER="$(FREELINX_TOOLCHAIN_DIR)/bin/clang" \
	-DCMAKE_C_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) $(FREELINX_RESOURCE_FLAGS) -O2" \
	-DCMAKE_C_LINK_EXECUTABLE="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) $(FREELINX_RESOURCE_FLAGS) -fuse-ld=lld --rtlib=compiler-rt <FLAGS> <LINK_FLAGS> <OBJECTS> <LIBRARIES> -o <TARGET>" \
	-DCMAKE_SYSROOT="$(FREELINX_SYSROOT)" \
	-DCMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES="$(FREELINX_SYSROOT)/include" \
	-DCMAKE_C_IMPLICIT_LINK_DIRECTORIES="$(FREELINX_SYSROOT)/lib" \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_BUILD_TYPE=Release \
	-DFT_DISABLE_BZIP2=ON \
	-DFT_DISABLE_BROTLI=ON \
	-DFT_DISABLE_HARFBUZZ=ON \
	-DFT_DISABLE_PNG=ON \
	-DZLIB_INCLUDE_DIR="$(ZLIB_PREFIX)/include" \
	-DZLIB_LIBRARY="$(ZLIB_PREFIX)/lib/libz.a"

# ZLIB_INCLUDE_DIR is not optional either.  Left to itself freetype runs
# find_package(ZLIB), finds the *host's* /usr/lib/libz.so, caches
# ZLIB_INCLUDE_DIR=/usr/include, and hands that to every compile line -- which
# is how a musl freetype ends up reading glibc's <stdio.h>.  Point it at the
# zlib that FreeLinX/ports already built.

# CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES is not cosmetic.  CMake probes the
# compiler with `clang -E -v` *without* --sysroot, records /usr/include as an
# implicit include directory, and then passes it back on every compile line as
# -isystem.  The host's <stdio.h> then wins over the sysroot's and freetype
# fails with "unknown type name '__gnuc_va_list'" -- glibc's stdio needs a
# glibc <stdarg.h> that is nowhere on a musl command line.  Naming the
# sysroot's own include directory is what stops the host one being re-added.

PROJECT_CFG_CMDS = \
	mkdir -p "$(SRC_TREE)/build" && \
	cd "$(SRC_TREE)/build" && \
	cmake .. $(FREETYPE_CMAKE_FLAGS)

BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && \
	make -j"$(FREELINX_JOBS)" freetype

.PHONY: install
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(STAGE_ROOT)/usr/lib/libfreetype.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/freetype"
	@cp -r "$(SRC_TREE)/include/freetype/." "$(STAGE_ROOT)/usr/include/freetype/"
	@$(INSTALL) -m 644 "$(SRC_TREE)/include/ft2build.h" "$(STAGE_ROOT)/usr/include/ft2build.h"
	@printf '[FreeLinX/ports] staged freetype2\n'
