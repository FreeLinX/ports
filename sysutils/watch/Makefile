# FreeLinX/ports - sysutils/watch : minimal watch utility.
#
# Metadata:
#   NAME       watch
#   CATEGORY   sysutils
#   SOURCE     none -- small original FreeLinX script (see distinfo)
#   LICENSE    0BSD (trivial, no copyrightable content)
#   INSTALL    staging/usr/bin/watch  ->  FreeLinX/src/rootfs/usr/bin/watch
#
# FreeLinX-original POSIX sh watch(1). Repeatedly runs a command, clearing
# the screen between invocations. Supports -n <seconds> for custom intervals.

NAME    := watch
VERSION := 1
CATEGORY:= sysutils
LICENSE := 0BSD
DEPENDENCIES:=
BUILD_DEPENDENCIES:=

INSTALL_BIN     := watch
INSTALL_RELPATH := usr/bin/watch

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))../..)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

SRC_ROOT = $(FREELINX_BUILD_DIR)/work/watch
BUILD_BIN = $(SRC_ROOT)/watch

.PHONY: do-prepare do-build

do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_ROOT)/.flx-extracted" ]; then \
		printf '[FreeLinX/ports] writing watch (no upstream to fetch)\n'; \
		mkdir -p "$(SRC_ROOT)"; \
		printf '%s\n' '#!/bin/sh' \
 '# watch -- FreeLinX minimal watch utility.' \
 '# Repeatedly executes a command, clearing the screen between runs.' \
 '# Usage: watch [-n seconds] command...' \
 'interval=2' \
 'while [ "$$#" -gt 0 ]; do' \
 '  case "$$1" in' \
 '    -n) shift; interval="$$1"; shift ;;' \
 '    -h|--help) printf "Usage: watch [-n seconds] command...\n"; exit 0 ;;' \
 '    -*) printf "watch: unknown option: %s\n" "$$1" >&2; exit 1 ;;' \
 '    *) break ;;' \
 '  esac' \
 'done' \
 'if [ "$$#" -eq 0 ]; then' \
 '  printf "watch: no command specified\n" >&2; exit 1' \
 'fi' \
 'while true; do' \
 '  printf "\033[H\033[2J"' \
 '  eval "$$@"' \
 '  sleep "$$interval"' \
 'done' > "$(SRC_ROOT)/watch"; \
		chmod +x "$(SRC_ROOT)/watch"; \
		touch "$(SRC_ROOT)/.flx-extracted"; \
	fi

$(BUILD_BIN): do-prepare
	@printf '[FreeLinX/ports] watch ready (no compile needed): %s\n' "$(BUILD_BIN)"

do-build: $(BUILD_BIN)

.PHONY: all clean install
all: do-build

install: $(STAGE_FILE)

clean:
	@true

-include $(CURDIR)/distinfo
