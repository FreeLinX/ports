# FreeLinX/ports - base/llvm-binutils : LLVM binutils equivalents (exposure).
#
# Per Phase 4, GNU binutils is NOT ported.  Instead this port exposes the
# FreeLinX LLVM toolchain's binutils-equivalent utilities under the canonical
# GNU names inside the FreeLinX rootfs (bin/ar, bin/nm, bin/objcopy, ...).
#
# Each staged name is a symlink to the matching FreeLinX LLVM tool:
#   ar       -> llvm-ar        nm        -> llvm-nm
#   objcopy  -> llvm-objcopy   objdump   -> llvm-objdump
#   readelf  -> llvm-readelf   size      -> llvm-size
#   strings  -> llvm-strings   strip     -> llvm-strip
#   ranlib   -> llvm-ranlib
#
# RESOLVED: the FreeLinX toolchain's host LLVM utilities were dynamically linked
# against GLIBC_2.38 (host has 2.36), so they failed to execute.  Fixed by
# bundling a GLIBC 2.41 runtime in $(FREELINX_TOOLCHAIN_DIR)/host-glibc and
# patching these binaries' ELF interpreter/rpath to use it (no system glibc
# change, no LD_PRELOAD).  All nine tools now run on the build host and are
# verified against real objects/archives.  Provenance is the FreeLinX LLVM 21.1.8
# install; only the host-side loader/runtime was complemented.

NAME   := llvm-binutils
VERSION:= 21.1.8
CATEGORY:= base
LICENSE:= Apache-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= FreeLinX LLVM toolchain (clang/LLD/binutils-equivalents)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ar
INSTALL_RELPATH:= bin/ar

# The GNU-name -> FreeLinX LLVM-tool mapping.
FLX_LLVM_TOOLS = ar nm objcopy objdump readelf size strings strip ranlib

include ../../mk/port.mk

# We expose the toolchain's own binaries; BUILD_BIN is the primary staged link.
BUILD_BIN = $(FREELINX_STAGING_DIR)/bin/ar

.PHONY: do-build
do-build:
	@set -e; \
	mkdir -p "$(FREELINX_STAGING_DIR)/bin"; \
	for t in $(FLX_LLVM_TOOLS); do \
		ln -sf "$(FREELINX_TOOLCHAIN_BIN)/llvm-$$t" "$(FREELINX_STAGING_DIR)/bin/$$t"; \
		printf '[FreeLinX/ports] expose bin/%s -> %s\n' "$$t" "$(FREELINX_TOOLCHAIN_BIN)/llvm-$$t"; \
	done
