# FreeLinX/ports - devel/libgc : Boehm-Demers-Weiser GC (8.2.8).
#
# Conservative garbage collector. Required by w3m.
# BSD-style license. Autotools build.

NAME    := libgc
VERSION := 8.2.8
CATEGORY:= devel
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/bdwgc-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/.libs/libgc.a

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl

PROJECT_CFG_CMDS = \
	cd "$(SRC_TREE)" && \
	if [ ! -f configure ]; then \
		autoreconf -fi 2>/dev/null || true; \
	fi && \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--enable-static \
		--disable-shared \
		--disable-cplusplus \
		--disable-gcj-support \
		--disable-java-support \
		--disable-sigsetjmp \
		--without-pic \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	make -j"$(FREELINX_JOBS)" all

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(SRC_TREE)/.libs/libgc.a" "$(STAGE_ROOT)/usr/lib/libgc.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/gc"
	@$(INSTALL) -m 644 "$(SRC_TREE)/include/gc/gc.h" "$(STAGE_ROOT)/usr/include/gc/gc.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/include/gc/gc_mark.h" "$(STAGE_ROOT)/usr/include/gc/gc_mark.h"
	@$(INSTALL) -m 644 "$(SRC_TREE)/include/gc/gc_version.h" "$(STAGE_ROOT)/usr/include/gc/gc_version.h"
	@printf '[FreeLinX/ports] staged libgc\n'
