# FreeLinX/ports - devel/meson : Meson 1.6.0 (build system, Python program).
#
# Meson is a build system written in Python (Apache-2.0). It is a plain Python
# program with no compilation step: the source distribution is a self-contained
# `meson.py` entry point plus the `mesonbuild/` package. It runs under the host
# python3 interpreter, so the build here is trivial: copy the files into the
# staging tree and emit a small /usr/bin/meson wrapper that invokes python3.
#
# This uses the install-only model (from sysutils/clear): only mk/install.mk
# conventions are needed; there is nothing to compile and no toolchain gate.

NAME    := meson
VERSION := 1.6.0
CATEGORY:= devel
LICENSE := Apache-2.0
DEPENDENCIES:= python3 (host interpreter)
BUILD_DEPENDENCIES:= python3 (host interpreter)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= meson
INSTALL_RELPATH:= usr/share/meson

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))../..)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/meson-$(VERSION)
# Source is present in the dist tree; fetch/extract into SRC_TREE so BUILD_BIN
# (SRC_TREE/meson.py) is a real input the staging rule can depend on.
DIST_TGZ = $(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)

# No-op configure/build: nothing to compile.
PROJECT_CFG_CMDS = :
BUILD_CMDS = :
BUILD_BIN = $(SRC_TREE)/meson.py

# do-prepare: fetch + extract the plain Python source (idempotent).
.PHONY: do-prepare do-build all install clean

do-prepare:
	@set -e; \
	printf '[FreeLinX/ports] preparing meson %s\n' '$(VERSION)'; \
	mkdir -p "$(FREELINX_DIST_DIR)"; \
	if [ ! -f "$(DIST_TGZ)" ]; then \
		(cd "$(FREELINX_DIST_DIR)" && curl -fL -o "$(DISTINFO_ARCHIVE)" "$(DISTINFO_URL)"); \
		_c="$$(sha256sum "$(DIST_TGZ)" | awk '{print $$1}')"; \
		if [ "$$_c" != "$(DISTINFO_SHA256)" ]; then \
			printf '[FreeLinX/ports][error] sha256 mismatch for %s (got %s)\n' "$(DIST_TGZ)" "$$_c"; \
			exit 1; \
		fi; \
	fi; \
	if [ ! -d "$(SRC_TREE)" ]; then \
		mkdir -p "$(FREELINX_BUILD_DIR)/work/$(NAME)"; \
		tar -xzf "$(DIST_TGZ)" -C "$(FREELINX_BUILD_DIR)/work/$(NAME)"; \
	fi

$(BUILD_BIN): do-prepare
	@test -f "$(BUILD_BIN)" || { \
		printf '[FreeLinX/ports][error] meson.py missing in %s\n' "$(SRC_TREE)"; \
		exit 1; }

do-build: $(BUILD_BIN)

# Custom install: copy meson.py + mesonbuild/ into usr/share/meson and write the
# /usr/bin/meson wrapper. Depend on the staged share file to ensure sources exist.
STAGE_SHARE = $(STAGE_ROOT)/usr/share/meson/meson.py

$(STAGE_SHARE): $(BUILD_BIN)
	@printf '[FreeLinX/ports] staging meson sources -> %s\n' "$(STAGE_ROOT)/usr/share/meson"
	@$(MKDIR) -p "$(STAGE_ROOT)/usr/share/meson"
	@$(INSTALL) -m 644 "$(SRC_TREE)/meson.py" "$(STAGE_ROOT)/usr/share/meson/meson.py"
	@cp -r "$(SRC_TREE)/mesonbuild" "$(STAGE_ROOT)/usr/share/meson/"

all: do-build

install: $(STAGE_SHARE)
	@printf '[FreeLinX/ports] writing /usr/bin/meson wrapper\n'
	@$(MKDIR) -p "$(STAGE_ROOT)/usr/bin"
	@printf '#!/bin/sh\nexec python3 /usr/share/meson/meson.py "$$@"\n' > "$(STAGE_ROOT)/usr/bin/meson"
	@chmod 755 "$(STAGE_ROOT)/usr/bin/meson"

clean:
	@true

-include $(CURDIR)/distinfo
