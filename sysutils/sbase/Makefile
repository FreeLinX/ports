# FreeLinX/ports - sysutils/sbase : suckless coreutils (git snapshot).
#
# Portable suckless coreutils: cat, cp, ls, mkdir, rm, etc.
# MIT license. POSIX make build.

NAME    := sbase
VERSION := 05222026
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/bin

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/sbase-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/cat

SBASE_BINS = cat cp echo hostname kill ln ls mkdir mv pwd rm rmdir \
             dirname basename head tail wc sort tr uniq tee chmod \
             chown chgrp id expr test touch sleep yes nohup \
             cksum cmp comm cut diff expand fold join paste readlink \
             seq split strings xargs printf env

# Write cross-compilation config.mk before building.
PROJECT_CFG_CMDS = \
	printf 'VERSION = %s\nPREFIX = /usr\nMANPREFIX = $$(PREFIX)/share/man\nCC = %s\nAR = %s\nRANLIB = %s\nCFLAGS = -O2\nCPPFLAGS = -D_DEFAULT_SOURCE -D_NETBSD_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700\nLDFLAGS = -static -fuse-ld=lld --rtlib=compiler-rt\nSMAKE = $$(MAKE)\n' \
		"$(VERSION)" "$(FREELINX_CC)" "$(FREELINX_AR)" \
		"$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" > "$(SRC_TREE)/config.mk"

# Build all binaries. sbase's Makefile builds one at a time via .c: rules.
BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	for bin in $(SBASE_BINS); do \
		$(MAKE) CC="$(FREELINX_CC)" AR="$(FREELINX_AR)" \
			CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
			CPPFLAGS="-D_DEFAULT_SOURCE -D_NETBSD_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700" \
			LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt" \
			$$bin || true; \
	done

# Override install to stage all built binaries.
.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@for bin in $(SBASE_BINS); do \
		if [ -f "$(SRC_TREE)/$$bin" ] && file "$(SRC_TREE)/$$bin" | grep -q "ELF"; then \
			$(INSTALL) -m 755 "$(SRC_TREE)/$$bin" "$(STAGE_ROOT)/usr/bin/$$bin"; \
			printf '[FreeLinX/ports] staged sbase/%s\n' "$$bin"; \
		fi; \
	done
