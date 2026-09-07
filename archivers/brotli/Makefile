# FreeLinX/ports - archivers/brotli : brotli 1.1.0 (modern compression).
#
# Brotli compression library and tools. MIT license. CMake build.
# Builds brotli CLI tool and static libs.

NAME    := brotli
VERSION := 1.1.0
CATEGORY:= archivers
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= brotli
INSTALL_RELPATH:= usr/bin/brotli

BROTLI_PREFIX = $(FREELINX_BUILD_DIR)/deps/brotli

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/brotli-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/build/brotli
BUILD_BIN   = $(PROJECT_BIN)

BROTLI_CMAKE_FLAGS = \
	-DCMAKE_SYSTEM_NAME=Linux \
	-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
	-DCMAKE_C_COMPILER="$(FREELINX_TOOLCHAIN_DIR)/bin/clang" \
	-DCMAKE_C_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
	-DCMAKE_EXE_LINKER_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
	-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_BUILD_TYPE=Release \
	-DBUILD_SHARED_LIBS=OFF \
	-DBROTLI_DISABLE_TESTS=ON \
	-DBROTLI_DISABLE_EXAMPLES=ON

PROJECT_CFG_CMDS = \
	mkdir -p "$(SRC_TREE)/build" && \
	cd "$(SRC_TREE)/build" && \
	cmake .. $(BROTLI_CMAKE_FLAGS)

BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && \
	make -j"$(FREELINX_JOBS)" brotli brotlienc brotlidec brotlicommon

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@$(INSTALL) -m 755 "$(SRC_TREE)/build/brotli" "$(STAGE_ROOT)/usr/bin/brotli"
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(SRC_TREE)/build/libbrotlienc.a" "$(STAGE_ROOT)/usr/lib/libbrotlienc.a"
	@$(INSTALL) -m 644 "$(SRC_TREE)/build/libbrotlidec.a" "$(STAGE_ROOT)/usr/lib/libbrotlidec.a"
	@$(INSTALL) -m 644 "$(SRC_TREE)/build/libbrotlicommon.a" "$(STAGE_ROOT)/usr/lib/libbrotlicommon.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/brotli"
	@$(INSTALL) -m 644 "$(SRC_TREE)/c/include/brotli/decode.h" "$(STAGE_ROOT)/usr/include/brotli/decode.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/c/include/brotli/encode.h" "$(STAGE_ROOT)/usr/include/brotli/encode.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/c/include/brotli/types.h" "$(STAGE_ROOT)/usr/include/brotli/types.h"
	@printf '[FreeLinX/ports] staged brotli\n'
