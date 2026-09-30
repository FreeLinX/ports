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

# --- resource dir ------------------------------------------------------------
# The single most important flag in this file.
#
# compiler-rt -- the builtins archive that replaces libgcc on a musl target --
# is found by clang relative to its own resource dir.  The FreeLinX toolchain's
# `clang` is not a self-contained install, so `clang -print-resource-dir`
# answers with the *host's* /usr/lib/clang/<ver>, and the link then pulls in the
# host's compiler-rt.  The host's compiler-rt was built by GCC, so every single
# binary produced carries `GCC: (GNU) 16.1.1' in its .comment section even
# though nothing GCC-built was ever named on the command line.  The port links,
# runs, passes every test, and is quietly the wrong build.
#
# So: prefer a resource dir inside the toolchain that actually holds a
# clang-built compiler-rt, and only fall back to asking clang when it does not.
# The wildcard matches on lib/linux rather than on the archive itself because
# `$(dir)` leaves a trailing slash behind, which both `-resource-dir` and the
# $(FLX_RTDIR)/lib/*/... lookups downstream then treat as part of the path.
FREELINX_RESOURCE_DIR?=$(firstword $(patsubst %/lib/linux,%,$(wildcard $(FREELINX_TOOLCHAIN_DIR)/lib/clang/*/lib/linux)) $(shell $(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -print-resource-dir 2>/dev/null))
FREELINX_RESOURCE_FLAGS:=$(if $(FREELINX_RESOURCE_DIR),-resource-dir=$(FREELINX_RESOURCE_DIR))

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
FREELINX_CFLAGS=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) $(CFLAGS)
FREELINX_CPPFLAGS=$(CPPFLAGS)
FREELINX_LDFLAGS=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) $(FREELINX_LINK_FLAGS) $(LDFLAGS)

# --- C++ ---------------------------------------------------------------------
# libc++ is the only C++ standard library FreeLinX can use: libstdc++ is GCC's.
# Two flags are needed and neither is optional.
#
# -nostdinc++ drops the host's C++ include path.  A --target=x86_64-linux-musl
# build should not be looking at /usr/include/c++/16 anyway, but when it does,
# the port still compiles and still produces a binary carrying GCC's headers.
#
# -isystem points at the sysroot's libc++ headers explicitly, because for a
# non-native target clang does not add them on its own.
FREELINX_CXX_FLAGS?=-nostdinc++ -isystem $(FREELINX_SYSROOT)/include/c++/v1

# The compiler driver used for both compiling and linking.
# IMPORTANT: defined with :=, NOT ?=, because GNU make's built-in CC defaults
# to "cc" and would otherwise shadow the configured FreeLinX compiler. Override
# the toolchain via FREELINX_CC instead of CC.
CC:=$(FREELINX_CC)
