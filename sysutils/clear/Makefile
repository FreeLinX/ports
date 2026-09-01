# FreeLinX/ports - sysutils/clear : minimal screen-clear utility.
#
# Metadata:
#   NAME       clear
#   CATEGORY   sysutils
#   SOURCE     none -- small original FreeLinX script (see distinfo)
#   LICENSE    Public domain / 0BSD (trivial, no copyrightable content)
#   INSTALL    staging/usr/bin/clear  ->  FreeLinX/src/rootfs/usr/bin/clear
#
# Build model (same deviation as sysutils/pfetch): this is a single POSIX sh
# script with nothing to compile, so it does NOT include mk/port.mk (which
# gates every build on FREELINX_TOOLCHAIN_READY -- irrelevant here). It
# includes mk/install.mk directly for the shared staging/rootfs-install
# conventions.
#
# Why not a "real" clear (NetBSD's usr.bin/clear or ncurses' clear): those
# work by looking up the terminal's clear-screen capability via
# tgetent()/tputs() against a termcap/terminfo database and $TERM -- which
# requires porting an entire terminfo/curses library first. FreeLinX has
# neither yet. This ships the standard ANSI "ESC[H ESC[2J" sequence instead,
# which is correct for every terminal FreeLinX actually runs on right now
# (QEMU's serial and VGA consoles). Revisit this port if/when a
# terminfo/curses library is ported for other reasons.
#
# IMPORTANT ORDERING NOTE (see sysutils/pfetch's Makefile for the full
# story): NAME/INSTALL_BIN/INSTALL_RELPATH are set BEFORE the mk/common.mk +
# mk/install.mk includes below, since make freezes the staging target's
# path at parse time, not at recipe-execution time.

NAME    := clear
VERSION := 1
CATEGORY:= sysutils
LICENSE := 0BSD
DEPENDENCIES:=
BUILD_DEPENDENCIES:=

INSTALL_BIN     := clear
INSTALL_RELPATH := usr/bin/clear

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))../..)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

SRC_ROOT = $(FREELINX_BUILD_DIR)/work/clear
BUILD_BIN = $(SRC_ROOT)/clear

# ---------------------------------------------------------------------------
# do-prepare: write the script directly (no archive to extract -- there is
# no upstream). Idempotent via the same .flx-extracted sentinel convention
# used elsewhere, so repeated builds don't needlessly rewrite it.
# ---------------------------------------------------------------------------
.PHONY: do-prepare do-build

do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_ROOT)/.flx-extracted" ]; then \
		printf '[FreeLinX/ports] writing clear (no upstream to fetch)\n'; \
		mkdir -p "$(SRC_ROOT)"; \
		printf '#!/bin/sh\n# clear -- FreeLinX minimal screen-clear utility.\n# Emits the standard ANSI "move to home, erase display" escape sequence.\n# See this port'"'"'s Makefile for why this is not a terminfo-based clear.\nprintf '"'"'\\033[H\\033[2J'"'"'\n' > "$(SRC_ROOT)/clear"; \
		chmod +x "$(SRC_ROOT)/clear"; \
		touch "$(SRC_ROOT)/.flx-extracted"; \
	fi

$(BUILD_BIN): do-prepare
	@printf '[FreeLinX/ports] clear ready (no compile needed): %s\n' "$(BUILD_BIN)"

do-build: $(BUILD_BIN)

.PHONY: all clean install
all: do-build

install: $(STAGE_FILE)

clean:
	@true

-include $(CURDIR)/distinfo
