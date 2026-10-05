# FreeLinX/ports - devel/lua : lua 5.4.9 (embeddable scripting language).
#
# Lua interpreter and compiler. MIT license, ISO C, no dependencies.
# Plain Makefile build, no autoconf/cmake. Static binaries. Readline
# support is intentionally omitted, keeping this port dependency-free.

NAME    := lua
VERSION := 5.4.9
CATEGORY:= devel
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= lua
INSTALL_RELPATH:= usr/bin/lua
# The port builds two programs.  Without this, luac was staged and never
# installed: install-rootfs copies INSTALL_RELPATH and nothing else.
INSTALL_EXTRA_RELPATHS:= usr/bin/luac

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/lua-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/lua
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/"

# Lua's own makefile drives the build; the linux target's -ldl/-Wl,-E
# defaults are overridden for a static musl link, and readline is left
# out (LUA_USE_LINUX only, no LUA_USE_READLINE).
BUILD_CMDS = \
	cd "$(SRC_TREE)/src" && \
	$(MAKE) all CC="$(FREELINX_CC)" \
		SYSCFLAGS="-DLUA_USE_LINUX" \
		SYSLIBS="" \
		MYCFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2 -std=gnu99" \
		MYLDFLAGS="$(FLX_LINK_FLAGS)" \
		MYLIBS=""

# lua builds two binaries: lua and luac.  Staged here as stage-extra
# rather than as an `install:` override, which would lose do-build and
# leave `make install` with nothing built.
.PHONY: stage-extra
stage-extra:
	@if [ -f "$(SRC_TREE)/src/luac" ]; then \
		mkdir -p "$(STAGE_ROOT)/usr/bin"; \
		install -m 755 "$(SRC_TREE)/src/luac" "$(STAGE_ROOT)/usr/bin/luac"; \
	fi
