# FreeLinX/ports - devel/readline : readline 8.2.13 (line editing).
#
# GNU readline line-editing library. GPL-3.0 license. Autoconf build, static.
# No curses dependency (--without-curses). Used by interactive shell tools.

NAME    := readline
VERSION := 8.2.13
CATEGORY:= devel
LICENSE := GPL-3.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= usr/lib

READLINE_PREFIX = $(FREELINX_BUILD_DIR)/deps/readline

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/readline-$(VERSION)
PROJECT_BIN = $(READLINE_PREFIX)/lib/libreadline.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(READLINE_PREFIX)" \
		--enable-static \
		--disable-shared \
		--without-curses \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libreadline.a"
	@mkdir -p "$(STAGE_ROOT)/usr/include"
	@$(INSTALL) -m 644 "$(READLINE_PREFIX)/include/readline/readline.h" "$(STAGE_ROOT)/usr/include/readline/readline.h"
	@$(INSTALL) -m 644 "$(READLINE_PREFIX)/include/readline/rlconf.h" "$(STAGE_ROOT)/usr/include/readline/rlconf.h"
	@$(INSTALL) -m 644 "$(READLINE_PREFIX)/include/readline/rlstdc.h" "$(STAGE_ROOT)/usr/include/readline/rlstdc.h"
	@printf '[FreeLinX/ports] staged readline\n'
