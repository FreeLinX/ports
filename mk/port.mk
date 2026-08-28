# -*- makefile -*-
# FreeLinX/ports - generic per-port rules shared by every port Makefile.
#
# A port Makefile only declares its metadata (NAME, VERSION, CATEGORY,
# LICENSE, SOURCE info) plus a build/install recipe using the variables below,
# then includes ../../mk/port.mk to get consistent:
#   - toolchain readiness gate (never builds if toolchain is absent)
#   - all / install / clean phony targets
#   - staging install rule
#
# This is NOT a package manager; it is a thin shared harness so port
# Makefiles stay small and reproducible.

# This file always lives at <root>/mk/port.mk, so the repo root is one level
# up from this file's own directory. MAKEFILE_LIST's last entry is port.mk at
# the time this line is read. `:=` fixes the value NOW (at parse time) so it
# does not drift to a different path when recipes reference it later.
FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/compiler.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

# Metadata defaults (leased from distinfo when present).
NAME?=
VERSION?=
CATEGORY?=
LICENSE?=
DEPENDENCIES?=
BUILD_DEPENDENCIES?=
TARGET?=$(FREELINX_TRIPLE)
PREFIX?=/

# Per-port version of the search path used by scripts (kept here for the
# list.sh / check.sh metadata summary; see scripts/).
PORT_FULL?=$(CATEGORY)/$(NAME)

# ---------------------------------------------------------------------------
# Build gate: refuse to pretend a build happened without a toolchain.
# ---------------------------------------------------------------------------
check-toolchain:
	@printf '[FreeLinX/ports] Checking toolchain...\n'
	@if [ "$(FREELINX_TOOLCHAIN_READY)" != yes ]; then \
	    printf '[FreeLinX/ports][error] toolchain not available: $(FREELINX_CC) / $(FREELINX_SYSROOT)\n'; \
	    printf '[FreeLinX/ports][error] bootstrap FreeLinX/toolchain first, or set FREELINX_CC/FREELINX_LD/FREELINX_SYSROOT\n'; \
	    exit 1; \
	fi
	@printf '[FreeLinX/ports] toolchain available\n'

.PHONY: all install check-toolchain clean do-build

all: check-toolchain do-build

# `do-build` is provided by the port (or by mk/base-port.mk for the simple
# utilities); see the make error if a port forgets to define it.

install: $(STAGE_PREFIX)/bin/$(INSTALL_BIN)

clean:
	@true

# ---------------------------------------------------------------------------
# Distinfo (source) metadata; populated/used by scripts and build recipes.
# ---------------------------------------------------------------------------
-include $(CURDIR)/distinfo
