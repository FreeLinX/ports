# FreeLinX/ports - security/libgcrypt : libgcrypt 1.11.0 (crypto library).
#
# LGPL-2.1-licensed general-purpose cryptographic library, used by GnuPG
# and other security tools.  Depends on libgpg-error (built as a separate
# port).  The configure step receives the libgpg-error prefix via
# --with-libgpg-error-prefix; the custom install: rule copies the archive
# and headers into the staging tree.

NAME    := libgcrypt
VERSION := 1.11.0
CATEGORY:= security
LICENSE := LGPL-2.1
DEPENDENCIES:= libgpg-error
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

GCRYPT_PREFIX  = $(FREELINX_BUILD_DIR)/deps/libgcrypt
GPG_ERROR_PREFIX = $(FREELINX_BUILD_DIR)/deps/libgpg-error

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libgcrypt-$(VERSION)
PROJECT_BIN = $(GCRYPT_PREFIX)/lib/libgcrypt.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	PATH="$(GPG_ERROR_PREFIX)/bin:$$PATH" \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(GCRYPT_PREFIX)" \
		--enable-static \
		--disable-shared \
		--with-libgpg-error-prefix="$(GPG_ERROR_PREFIX)" \
		GPG_ERROR_CFLAGS="-I$(GPG_ERROR_PREFIX)/include" \
		GPG_ERROR_LIBS="-L$(GPG_ERROR_PREFIX)/lib -lgpg-error" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(GPG_ERROR_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(GPG_ERROR_PREFIX)/lib"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(GCRYPT_PREFIX)/lib/libgcrypt.a" "$(STAGE_ROOT)/usr/lib/libgcrypt.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(GCRYPT_PREFIX)/include/gcrypt.h" "$(STAGE_ROOT)/usr/include/gcrypt.h"
	@printf '[FreeLinX/ports] staged libgcrypt\n'
