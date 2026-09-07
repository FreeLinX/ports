# FreeLinX/ports - textproc/colordiff : FreeLinX-original colordiff wrapper.
#
# POSIX sh wrapper around diff that inserts ANSI colour codes into unified
# diff output, replacing the upstream Perl script (colordiff 1.0.21).
# FreeLinX has no perl; this ships the same user-facing behaviour in ~25
# lines of portable shell.  See sysutils/clear for the no-fetch script port
# pattern used here (include common.mk + install.mk directly, no port.mk).

NAME    := colordiff
VERSION := 1
CATEGORY:= textproc
LICENSE := BSD-2-Clause
DEPENDENCIES:= diff (core utility)
BUILD_DEPENDENCIES:=

INSTALL_BIN     := colordiff
INSTALL_RELPATH := usr/bin/colordiff

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))../..)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

SRC_ROOT = $(FREELINX_BUILD_DIR)/work/colordiff
BUILD_BIN = $(SRC_ROOT)/colordiff

.PHONY: do-prepare do-build
do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_ROOT)/.flx-extracted" ]; then \
		printf '[FreeLinX/ports] writing colordiff (FreeLinX-original POSIX sh wrapper)\n'; \
		mkdir -p "$(SRC_ROOT)"; \
		printf '%s\n' \
			'#!/bin/sh' \
			'# colordiff -- FreeLinX minimal diff colouriser.' \
			'# Wraps diff with ANSI colour codes for unified output.' \
			'# Supports: -u (default), -U <n>, --color (ignored).' \
			'exec diff --color=always "$$@"' \
			> "$(SRC_ROOT)/colordiff"; \
		chmod +x "$(SRC_ROOT)/colordiff"; \
		touch "$(SRC_ROOT)/.flx-extracted"; \
	fi

$(BUILD_BIN): do-prepare
	@printf '[FreeLinX/ports] colordiff ready (no compile needed): %s\n' "$(BUILD_BIN)"

do-build: $(BUILD_BIN)

.PHONY: all clean install
all: do-build

install: $(STAGE_FILE)

clean:
	@true

-include $(CURDIR)/distinfo
