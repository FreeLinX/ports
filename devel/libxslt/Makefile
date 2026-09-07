# FreeLinX/ports - devel/libxslt : XSLT library (1.1.42).
#
# XSLT 1.0 processing library, derived from libxml2.
# MIT license. Autoconf build. Future dependency: libxml2.

NAME    := libxslt
VERSION := 1.1.42
CATEGORY:= devel
LICENSE := MIT
DEPENDENCIES:= libxml2
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

LIBXSLT_PREFIX = $(FREELINX_BUILD_DIR)/deps/libxslt

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libxslt-$(VERSION)
PROJECT_BIN = $(LIBXSLT_PREFIX)/lib/libxslt.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBXSLT_PREFIX)" \
		--enable-static \
		--disable-shared \
		--without-python \
		--without-crypto \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libxslt.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include/libxslt"
	@cp "$(LIBXSLT_PREFIX)/include/libxslt/xslt.h" "$(STAGE_ROOT)/usr/include/libxslt/xslt.h"
	@cp "$(LIBXSLT_PREFIX)/include/libxslt/xsltutils.h" "$(STAGE_ROOT)/usr/include/libxslt/xsltutils.h"
	@cp "$(LIBXSLT_PREFIX)/include/libxslt/xsltInternals.h" "$(STAGE_ROOT)/usr/include/libxslt/xsltInternals.h"
	@printf '[FreeLinX/ports] staged libxslt\n'
