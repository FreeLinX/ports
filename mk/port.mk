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
# The NetBSD compatibility overlay.
#
# base/compat/nbsys holds the NetBSD kernel-source headers and the public
# headers musl does not have (sys/audioio.h, sys/proc.h, db.h, rpc/...).  It is
# searched with -idirafter, never -I, and that is load-bearing: the overlay
# deliberately re-declares types musl also declares - struct timespec, sigset_t,
# the fixed-width integer typedefs, pid_t - because the NetBSD sources in this
# tree expect NetBSD's versions.  With -I the overlay would be found *first* and
# every one of those became a redefinition error; with -idirafter the musl
# sysroot wins every name clash and the overlay only fills the gaps.
#
# This lives in port.mk rather than base-port.mk because it applies to every
# port, including the project ports (lynx, dhcpcd) that drive their own
# configure and sub-make and so have to name the overlay themselves.  When it
# lived only in base-port.mk, a project port that wrote $(FLX_NBSYS_FLAGS) into
# its CPPFLAGS expanded it to nothing at all - a silently missing include path
# rather than a visible mistake.
# ---------------------------------------------------------------------------
FLX_COMPAT?=$(FREELINX_PORTS_ROOT)/base/compat
FLX_NBSYS       := $(FLX_COMPAT)/nbsys
FLX_NBSYS_FLAGS  = -idirafter $(FLX_NBSYS)/sys -idirafter $(FLX_NBSYS)/include

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
# The reasons are not all about the kernel.  A port declares this when FreeLinX
# cannot build it, whatever the obstacle: a third-party stack it does not ship
# (the cases this mechanism was introduced for), a toolchain that cannot produce
# the artefact at all (grub needs GCC for its freestanding boot code), or a
# required input that is not vendored and not fetched (linux-firmware's
# tarball).  So every message that reports a declaration says "cannot be built
# for FreeLinX" and quotes the port's own reason - the name is kept for
# compatibility with the 50+ ports that already carry it, but the wording must
# not imply the kernel is the cause when it is not.
#
# It is a statement about the *port*, not a suppression: the port stays in the
# tree, keeps its metadata, and still shows up in list.sh / check.sh.
# ---------------------------------------------------------------------------
PORT_NOT_PORTABLE?=
PORT_NOT_PORTABLE_REASON?=$(PORT_NOT_PORTABLE)

.PHONY: portability
portability:
	@if [ -n "$$PORT_NOT_PORTABLE" ]; then \
	    printf '[FreeLinX/ports] %s cannot be built for FreeLinX: %s\n' "$$NAME" "$$PORT_NOT_PORTABLE"; \
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

install: $(STAGE_FILE) install-aliases

clean:
	@true

# ---------------------------------------------------------------------------
# Distinfo (source) metadata; populated/used by scripts and build recipes.
# ---------------------------------------------------------------------------
-include $(CURDIR)/distinfo
