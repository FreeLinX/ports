# FreeLinX/ports - sysutils/ubase : suckless util-linux (git snapshot).
#
# Portable suckless util-linux: mount, umount, ps, id, login, su, dmesg, etc.
# MIT license. POSIX make build.

NAME    := ubase
VERSION := 05222026
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/bin

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/ubase-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/id

# Key ubase utilities for FreeLinX.
UBASE_BINS = chvt clear dd df dmesg eject free halt id insmod killall5 \
             login lsmod mesg mknod mount mountpoint nologin pidof ps \
             pwdx rmmod stat su swapon swapoff umount uptime who

PROJECT_CFG_CMDS = \
	printf 'VERSION = %s\nPREFIX = /usr\nMANPREFIX = $$(PREFIX)/share/man\nCC = %s\nAR = %s\nRANLIB = %s\nCPPFLAGS = -D_FILE_OFFSET_BITS=64 -D_XOPEN_SOURCE=700\nCFLAGS = -std=c99 -Wall -Wextra\nLDLIBS =\nLDFLAGS = -static -fuse-ld=lld --rtlib=compiler-rt\n' \
		"$(VERSION)" "$(FREELINX_CC)" "$(FREELINX_AR)" \
		"$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" > "$(SRC_TREE)/config.mk" && \
	touch "$(SRC_TREE)/config.h"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	for bin in $(UBASE_BINS); do \
		$(MAKE) CC="$(FREELINX_CC)" AR="$(FREELINX_AR)" \
			CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -std=c99 -Wall -Wextra" \
			CPPFLAGS="-D_XOPEN_SOURCE=700" \
			LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt" \
			LDLIBS="" \
			$$bin || true; \
	done

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@for bin in $(UBASE_BINS); do \
		if [ -f "$(SRC_TREE)/$$bin" ] && file "$(SRC_TREE)/$$bin" | grep -q "ELF"; then \
			$(INSTALL) -m 755 "$(SRC_TREE)/$$bin" "$(STAGE_ROOT)/usr/bin/$$bin"; \
			printf '[FreeLinX/ports] staged ubase/%s\n' "$$bin"; \
		fi; \
	done
