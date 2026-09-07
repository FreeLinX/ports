# FreeLinX/ports - net/busybox : busybox 1.36.1 (multi-call binary toolkit).
#
# Lightweight multi-call binary toolkit. GPL-2.0 license.
# Custom do-extract overrides the port framework's default extraction to
# handle .tar.bz2 archives explicitly (the default case statement does not
# have a *.tar.bz2 branch; the fallback tar -xf may or may not auto-detect
# bzip2 depending on the host tar build).
# Busybox's minimal build system is used; no autoconf/cmake.

NAME    := busybox
VERSION := 1.36.1
CATEGORY:= net
LICENSE := GPL-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= busybox
INSTALL_RELPATH:= usr/bin/busybox

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/busybox-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/busybox
BUILD_BIN   = $(PROJECT_BIN)

# ---------------------------------------------------------------------------
# do-extract: override for .tar.bz2 support.
# The port framework's default do-extract handles *.tar.xz and *.tar.gz
# but not *.tar.bz2.  Use tar -xjf explicitly for this port.
# ---------------------------------------------------------------------------
do-extract: do-fetch
	@set -e; \
	if [ ! -d "$(SRC_TREE)" ]; then \
		printf '[FreeLinX/ports] extracting %s (bz2 override)\n' "$(DISTINFO_ARCHIVE)"; \
		mkdir -p "$(SRC_DIR)"; \
		tar -xjf "$(DIST_TGZ)" -C "$(SRC_DIR)"; \
		if [ -d patches ]; then \
			printf '[FreeLinX/ports] applying patches for %s\n' "$(NAME)"; \
			for p in patches/*.patch; do \
				if grep -q '^--- ' "$$p" 2>/dev/null; then \
					patch -d "$(SRC_TREE)" -p1 < "$$p"; \
				fi; \
			done; \
		fi; \
	fi

PROJECT_CFG_CMDS = \
	touch "$(SRC_TREE)/"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" defconfig && \
	sed -i '/CONFIG_STATIC is not set/c\CONFIG_STATIC=y' "$(SRC_TREE)/.config" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt"
