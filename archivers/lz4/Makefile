# FreeLinX/ports - archivers/lz4 : lz4 1.10.0 (fast compression).
#
# LZ4 compression library and tool. BSD-2-Clause license. Plain Makefile build.
# Builds the lz4 CLI tool and stages liblz4.a static archive.

NAME    := lz4
VERSION := 1.10.0
CATEGORY:= archivers
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= lz4
INSTALL_RELPATH:= usr/bin/lz4

LZ4_PREFIX = $(FREELINX_BUILD_DIR)/deps/lz4

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/lz4-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/programs/lz4
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = :

BUILD_CMDS = \
	make -C "$(SRC_TREE)/programs" -j"$(FREELINX_JOBS)" lz4 \
		CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" && \
	make -C "$(SRC_TREE)/lib" -j"$(FREELINX_JOBS)" liblz4.a \
		CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2"

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@$(INSTALL) -m 755 "$(SRC_TREE)/programs/lz4" "$(STAGE_ROOT)/usr/bin/lz4"
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(SRC_TREE)/lib/liblz4.a" "$(STAGE_ROOT)/usr/lib/liblz4.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(SRC_TREE)/lib/lz4.h" "$(STAGE_ROOT)/usr/include/lz4.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/lib/lz4frame.h" "$(STAGE_ROOT)/usr/include/lz4frame.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/lib/lz4hc.h" "$(STAGE_ROOT)/usr/include/lz4hc.h"
	@printf '[FreeLinX/ports] staged lz4\n'
