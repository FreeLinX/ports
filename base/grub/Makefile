# FreeLinX/ports - base/grub : GRUB 2 bootloader build infrastructure.
#
# GRUB is treated as a BOOTLOADER, NOT runtime userland.  It must NOT be
# installed into rootfs/bin.  This port provides the boot-image build
# infrastructure (iso/mkiso.sh) and documents the exact build requirements.
#
# BLOCKER (see REPORT): building GRUB's utilities (grub-mkimage/grub-mkrescue)
# and its freestanding BIOS/UEFI boot code requires a GCC+binutils
# cross/cross-capable toolchain plus working objcopy/nm.  The FreeLinX
# clang+musl toolchain cannot build GRUB: its freestanding boot components are
# GCC-specific, and the toolchain's llvm-objcopy/llvm-nm cannot run on this
# build host (GLIBC_2.38 required, host has 2.36).  No fake ISO is produced.
#
# `make all` therefore reports the blocker and fails honestly instead of
# claiming a (fake) successful GRUB build.

NAME   := grub
VERSION:= 2.12
CATEGORY:= base
LICENSE:= GPL-3.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= GCC+binutils cross toolchain (NOT provided by FreeLinX clang+musl); working objcopy/nm
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

include ../../mk/port.mk

# The blocker above, stated where the build actually reads it.  Declaring it
# rather than only failing means a whole-tree run reports grub in the "cannot be
# built, and here is why" bucket instead of counting it as broken: it is not
# broken, it is a toolchain limitation, and a summary that calls it broken is
# reporting something untrue.  The do-build recipe below stays as well, so
# `make` in this directory still refuses out loud rather than doing nothing.
PORT_NOT_PORTABLE:= needs a GCC+binutils cross toolchain for its freestanding boot code, and working objcopy/nm; the FreeLinX clang+musl toolchain provides neither, and llvm-objcopy/llvm-nm do not run on this build host (GLIBC_2.38 required, host has 2.36)

.PHONY: do-build
do-build:
	@printf '[FreeLinX/ports][error] GRUB cannot be built with the FreeLinX clang+musl toolchain.\n'
	@printf '[FreeLinX/ports][error] Requires GCC+binutils cross toolchain and working objcopy/nm (llvm-objcopy/llvm-nm are non-functional on this GLIBC 2.36 host).\n'
	@printf '[FreeLinX/ports][error] Boot-image infrastructure is staged under base/grub/iso; no ISO is fabricated.\n'
	@exit 1
