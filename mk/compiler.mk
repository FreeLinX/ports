# -*- makefile -*-
# FreeLinX/ports - compiler/linker flags for building against the FreeLinX
# toolchain.
#
# Everything is expressed in terms of the configurable variables defined in
# config/default.conf (see mk/common.mk). No hard-coded toolchain path is ever
# referenced here.
#
# The canonical invocation is conceptually:
#
#     clang --target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) ...
#
# with LLD selected via -fuse-ld=lld. Static linking is configurable through
# FREELINX_LINK_MODE and defaults to "static" during bootstrap. It is NOT
# forced for every future package: set FREELINX_LINK_MODE=dynamic (or a per
# port MAKE= variable) to produce dynamically-linked output instead.

# --- toolchain readiness ----------------------------------------------------
# Expands to "yes" only when the configured compiler and sysroot actually
# exist. Used by ports to bail with a clear message instead of pretending the
# build happened. Implemented as a recursive shell expression so it stays on
# one line and works in both GNU make and bmake.
FREELINX_TOOLCHAIN_READY?=$(shell if [ -n "$(FREELINX_CC)" ] && [ -x "$(FREELINX_CC)" ] && [ -d "$(FREELINX_SYSROOT)" ]; then printf yes; else printf no; fi)

# --- compiler flags ---------------------------------------------------------
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=

# Target and sysroot are always applied when the values are configured.
FREELINX_TARGET_FLAGS:=$(if $(FREELINX_TRIPLE),--target=$(FREELINX_TRIPLE))
FREELINX_SYSROOT_FLAGS:=$(if $(FREELINX_SYSROOT),--sysroot=$(FREELINX_SYSROOT))

# Link with LLD (the FreeLinX linker). On by default -- correct for the real
# FreeLinX toolchain -- but can be disabled (FREELINX_LLD=no) on hosts lacking
# lld while testing the framework plumbing.
FREELINX_LLD?=yes

ifeq ($(FREELINX_LINK_MODE),static)
    FREELINX_LINK_FLAGS+=-static
else
    # Dynamic linking: no -static.
endif
ifeq ($(FREELINX_LLD),yes)
    FREELINX_LINK_FLAGS+=-fuse-ld=lld
endif

# Aggregate flag sets that ports should use for their compile/link steps.
FREELINX_CFLAGS=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(CFLAGS)
FREELINX_CPPFLAGS=$(CPPFLAGS)
FREELINX_LDFLAGS=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_LINK_FLAGS) $(LDFLAGS)

# The compiler driver used for both compiling and linking.
# IMPORTANT: defined with :=, NOT ?=, because GNU make's built-in CC defaults
# to "cc" and would otherwise shadow the configured FreeLinX compiler. Override
# the toolchain via FREELINX_CC instead of CC.
CC:=$(FREELINX_CC)
