# FreeLinX/ports - devel/pcre2 : Perl-compatible regular expressions (10.44).
#
# PCRE2 library, BSD-3-Clause license. Build-time dependency for grep-like
# tools and other regex consumers. Autoconf configure, static only, no
# GNU runtime or tool in the system or the build.

NAME    := pcre2
VERSION := 10.44
CATEGORY:= devel
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

PCRE2_PREFIX = $(FREELINX_BUILD_DIR)/deps/pcre2

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/pcre2-$(VERSION)
PROJECT_BIN = $(PCRE2_PREFIX)/lib/libpcre2-8.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(PCRE2_PREFIX)" \
		--enable-static \
		--disable-shared \
		--disable-pcre2-16 \
		--disable-pcre2-32 \
		--enable-pcre2-8 \
		--disable-jit \
		--disable-pcre2grep-callout \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libpcre2-8.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(PCRE2_PREFIX)/include/pcre2.h" "$(STAGE_ROOT)/usr/include/pcre2.h"
	@printf '[FreeLinX/ports] staged pcre2\n'