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
# Portability declaration.
#
# FreeLinX runs a Linux kernel, so a good part of the NetBSD base set cannot
# work here at all: it is written against NetBSD kernel internals (kvm/kinfo,
# BSD mbufs, BSD quotas, tape ioctls, the rump subsystem, ...) or against
# third-party stacks FreeLinX does not ship (Bluetooth, Kerberos, libaudio).
# Those ports used to be indistinguishable from real regressions: they simply
# failed, one after another, with compiler errors deep in a NetBSD header.
#
# A port in that category sets PORT_NOT_PORTABLE to a one-line reason.  The
# build gate then refuses with that reason instead of a confusing clang error,
# and `make build` (all ports) skips it and counts it, so a run of the whole
# tree reports the real number of buildable ports.
#
# It is a statement about the *port*, not a suppression: the port stays in the
# tree, keeps its metadata, and still shows up in list.sh / check.sh.
# ---------------------------------------------------------------------------
PORT_NOT_PORTABLE?=
PORT_NOT_PORTABLE_REASON?=$(PORT_NOT_PORTABLE)

.PHONY: portability
portability:
	@if [ -n "$$PORT_NOT_PORTABLE" ]; then \
	    printf '[FreeLinX/ports] %s is not portable to a Linux kernel: %s\n' "$$NAME" "$$PORT_NOT_PORTABLE"; \
	fi

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

# Portability gate. Runs before the toolchain gate so the message names the
# real blocker (a NetBSD-kernel dependency) rather than a missing compiler.
#
# The name and the reason are read from the environment, not spliced into the
# shell command.  A reason is prose, so it contains apostrophes - "struct
# stat's st_flags" - and interpolating one into a single-quoted shell string
# closes the quote early and turns this gate into a syntax error, which reads
# as a broken port instead of the deliberate refusal it is.
check-portable:
	@if [ -n "$$PORT_NOT_PORTABLE" ]; then \
	    printf '[FreeLinX/ports][error] %s cannot be built for FreeLinX: %s\n' "$$NAME" "$$PORT_NOT_PORTABLE"; \
	    printf '[FreeLinX/ports][error] this port targets NetBSD kernel internals/third-party stacks that have no Linux equivalent; it is expected to fail, not a regression\n'; \
	    exit 1; \
	fi

export NAME
export PORT_NOT_PORTABLE

.PHONY: all install check-toolchain check-portable clean do-build

all: check-portable check-toolchain do-build

# `do-build` is provided by the port (or by mk/base-port.mk for the simple
# utilities); see the make error if a port forgets to define it.

install: $(STAGE_FILE)

clean:
	@true

# ---------------------------------------------------------------------------
# Distinfo (source) metadata; populated/used by scripts and build recipes.
# ---------------------------------------------------------------------------
-include $(CURDIR)/distinfo
