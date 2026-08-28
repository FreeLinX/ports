# FreeLinX/ports - shells/netbsd-sh : NetBSD `/bin/sh` (Almquist shell, ash).
#
# This is the FIRST real FreeLinX port. It uses the NetBSD upstream sh, NOT an
# invented shell. Source: NetBSD base src set, external/bsd/bin/sh.
#
# Metadata:
#   NAME       netbsd-sh
#   VERSION    10.1        (NetBSD release the source is drawn from)
#   CATEGORY   shells
#   SOURCE     NetBSD external/bsd/bin/sh (see distinfo)
#   LICENSE    BSD-2-Clause
#   TARGET     x86_64-linux-musl
#   INSTALL    staging/bin/sh  ->  FreeLinX/src/rootfs/bin/sh
#
# Build model:
#   clang --target=x86_64-linux-musl --sysroot=$(FREELINX_SYSROOT)
#         -fuse-ld=lld [-static] external/bsd/bin/sh/*.c ...
#
# Known compatibility concerns (to be resolved with small patches, in patches/,
# ONLY as the toolchain becomes available -- do not invent giant hacks):
#   - BSD-only libc surface (strlcpy/strlcat, err(), etc.): check musl.
#   - NBQUEUE/queue.h and other NetBSD internal headers: musl does not ship
#     BSD headers; minimal vendored copies or patches are the path.
#   - The sh build uses NetBSD bmake; we compile the *.c directly instead of
#     wiring the whole NetBSD buildsystem.

NAME   := netbsd-sh
VERSION:= 10.1
CATEGORY:= shells
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Install destination, staged as bin/sh -> staging/bin/sh -> src/rootfs/bin/sh.
# NOTE: INSTALL_BIN / INSTALL_RELPATH must be set BEFORE including mk/port.mk:
# make freezes rule-target names at parse time, so a value set after the include
# would not reach the staging rule.
INSTALL_BIN:= sh
INSTALL_RELPATH:= bin/sh

include ../../mk/port.mk

# Where the fetched NetBSD src set is unpacked and the sh sources live.
SRC_ROOT = $(FREELINX_BUILD_DIR)/work/netbsd-sh
SH_SRC   = $(SRC_ROOT)/external/bsd/bin/sh

# Built binary (source of the staging copy).
BUILD_BIN = $(SRC_ROOT)/sh

.PHONY: do-build
do-build:
	@printf '[FreeLinX/ports] Building netbsd-sh...\n'
	@printf '[FreeLinX/ports][error] NOT BUILT: FreeLinX toolchain is not available yet\n'
	@printf '[FreeLinX/ports][error]   compiler: $(FREELINX_CC)\n'
	@printf '[FreeLinX/ports][error]   sysroot : $(FREELINX_SYSROOT)\n'
	@printf '[FreeLinX/ports][error]   source  : $(SH_SRC)\n'
	@printf '[FreeLinX/ports][error] Bootstrap FreeLinX/toolchain (clang+LLD+musl sysroot), then\n'
	@printf '[FreeLinX/ports][error] populate this recipe (see README "First port: NetBSD sh").\n'
	@exit 1
