# FreeLinX/ports - sysutils/pstree : psmisc 23.6 (process tree viewer).
#
# Displays the process tree. Also builds fuser and killall as multi-binary
# port (following the runit multi-binary pattern).
# GPL-2.0 license. Autoconf build.

NAME    := pstree
VERSION := 23.6
CATEGORY:= sysutils
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= pstree
INSTALL_RELPATH:= usr/bin/pstree

include ../../mk/project-port.mk

NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/ncurses

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/psmisc-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/pstree
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(NCURSES_PREFIX)/include -I$(NCURSES_PREFIX)/include/ncursesw" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib"

# ---------------------------------------------------------------------------
# Extra staged binaries beyond INSTALL_BIN/INSTALL_RELPATH.
# psmisc builds pstree, fuser, killall (and others we don't stage).
# Follows the runit multi-binary pattern from sysutils/runit.
# ---------------------------------------------------------------------------
install: $(STAGE_ROOT)/usr/bin/fuser $(STAGE_ROOT)/usr/bin/killall

$(STAGE_ROOT)/usr/bin/fuser: $(SRC_TREE)/src/fuser
	@printf '[FreeLinX/ports] staging fuser -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@

$(STAGE_ROOT)/usr/bin/killall: $(SRC_TREE)/src/killall
	@printf '[FreeLinX/ports] staging killall -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@
