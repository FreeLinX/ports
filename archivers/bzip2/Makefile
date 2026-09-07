# FreeLinX/ports - archivers/bzip2 : bzip2 1.0.8 (block-sorting compressor).
#
# bzip2 compression utility. BSD-3-Clause license. Plain Makefile build,
# no autoconf/cmake. Static binary. Also stages bzip2recover, bunzip2, bzcat.

NAME    := bzip2
VERSION := 1.0.8
CATEGORY:= archivers
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= bzip2
INSTALL_RELPATH:= usr/bin/bzip2

BZIP2_PREFIX = $(FREELINX_BUILD_DIR)/deps/bzip2

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/bzip2-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/bzip2
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = :

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" clean all \
		CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
		LDFLAGS-STATIC="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" && \
	make -j"$(FREELINX_JOBS)" bzip2recover \
		CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
		LDFLAGS-STATIC="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt"

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@$(INSTALL) -m 755 "$(SRC_TREE)/bzip2" "$(STAGE_ROOT)/usr/bin/bzip2"
	@$(INSTALL) -m 755 "$(SRC_TREE)/bzip2recover" "$(STAGE_ROOT)/usr/bin/bzip2recover"
	@ln -sf bzip2 "$(STAGE_ROOT)/usr/bin/bunzip2"
	@ln -sf bzip2 "$(STAGE_ROOT)/usr/bin/bzcat"
	@ln -sf bzip2 "$(STAGE_ROOT)/usr/bin/bzcmp"
	@ln -sf bzip2 "$(STAGE_ROOT)/usr/bin/bzdiff"
	@ln -sf bzip2 "$(STAGE_ROOT)/usr/bin/bzgrep"
	@ln -sf bzip2 "$(STAGE_ROOT)/usr/bin/bzless"
	@ln -sf bzip2 "$(STAGE_ROOT)/usr/bin/bzmore"
	@printf '[FreeLinX/ports] staged bzip2\n'
