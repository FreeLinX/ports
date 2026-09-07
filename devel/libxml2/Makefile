# FreeLinX/ports - devel/libxml2 : libxml2 2.13.5 (XML parser).
#
# GNOME XML parser library. MIT license. Autoconf build, static only.
# No Python, no lzma. Used by curl and other XML consumers.

NAME    := libxml2
VERSION := 2.13.5
CATEGORY:= devel
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= usr/lib

LIBXML2_PREFIX = $(FREELINX_BUILD_DIR)/deps/libxml2

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libxml2-$(VERSION)
PROJECT_BIN = $(LIBXML2_PREFIX)/lib/libxml2.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(LIBXML2_PREFIX)" \
		--enable-static \
		--disable-shared \
		--without-python \
		--without-lzma \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libxml2.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(LIBXML2_PREFIX)/include/libxml2/libxml/encoding.h" "$(STAGE_ROOT)/usr/include/encoding.h"
	@$(INSTALL) -m 644 "$(LIBXML2_PREFIX)/include/libxml2/libxml/parser.h" "$(STAGE_ROOT)/usr/include/parser.h"
	@$(INSTALL) -m 644 "$(LIBXML2_PREFIX)/include/libxml2/libxml/tree.h" "$(STAGE_ROOT)/usr/include/tree.h"
	@$(INSTALL) -m 644 "$(LIBXML2_PREFIX)/include/libxml2/libxml/xmlerror.h" "$(STAGE_ROOT)/usr/include/xmlerror.h"
	@$(INSTALL) -m 644 "$(LIBXML2_PREFIX)/include/libxml2/libxml/xmlexports.h" "$(STAGE_ROOT)/usr/include/xmlexports.h"
	@$(INSTALL) -m 644 "$(LIBXML2_PREFIX)/include/libxml2/libxml/xmlversion.h" "$(STAGE_ROOT)/usr/include/xmlversion.h"
	@$(INSTALL) -m 644 "$(LIBXML2_PREFIX)/include/libxml2/libxml/xmlstring.h" "$(STAGE_ROOT)/usr/include/xmlstring.h"
	@printf '[FreeLinX/ports] staged libxml2\n'
