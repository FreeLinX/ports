# FreeLinX/ports - net/netcat : minimal POSIX sh netcat.
#
# Metadata:
#   NAME       nc (netcat)
#   CATEGORY   net
#   SOURCE     none -- small original FreeLinX script (see distinfo)
#   LICENSE    0BSD (trivial, no copyrightable content)
#   INSTALL    staging/usr/bin/nc  ->  FreeLinX/src/rootfs/usr/bin/nc
#
# FreeLinX-original POSIX sh netcat(1). Provides basic TCP/UDP client
# connectivity. Uses printf/read for I/O and shell redirections for
# TCP connections. This is a minimal stopgap; a real netcat will be
# ported when upstream sources become reliably available.

NAME    := netcat
VERSION := 1
CATEGORY:= net
LICENSE := 0BSD
DEPENDENCIES:=
BUILD_DEPENDENCIES:=

INSTALL_BIN     := nc
INSTALL_RELPATH := usr/bin/nc

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))../..)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

SRC_ROOT = $(FREELINX_BUILD_DIR)/work/netcat
BUILD_BIN = $(SRC_ROOT)/nc

.PHONY: do-prepare do-build

do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_ROOT)/.flx-extracted" ]; then \
		printf '[FreeLinX/ports] writing netcat (no upstream to fetch)\n'; \
		mkdir -p "$(SRC_ROOT)"; \
		printf '%s\n' '#!/bin/sh' \
 '# nc -- FreeLinX minimal netcat.' \
 '# Basic TCP/UDP client for scripting.' \
 '# Usage: nc [-u] host port' \
 'udp=0' \
 'while [ "$$#" -gt 0 ]; do' \
 '  case "$$1" in' \
 '    -u) udp=1; shift ;;' \
 '    -h|--help) printf "Usage: nc [-u] host port\n"; exit 0 ;;' \
 '    -*) printf "nc: unknown option: %s\n" "$$1" >&2; exit 1 ;;' \
 '    *) break ;;' \
 '  esac' \
 'done' \
 'if [ "$$#" -lt 2 ]; then' \
 '  printf "Usage: nc [-u] host port\n" >&2; exit 1' \
 'fi' \
 'host="$$1"; port="$$2"' \
 'if [ "$$udp" -eq 1 ]; then' \
 '  exec 3<>/dev/udp/"$$host"/"$$port"' \
 'else' \
 '  exec 3<>/dev/tcp/"$$host"/"$$port"' \
 'fi' \
 'while read -r line <&3; do' \
 '  printf "%s\n" "$$line"' \
 'done' \
 'exec 3>&-' > "$(SRC_ROOT)/nc"; \
		chmod +x "$(SRC_ROOT)/nc"; \
		touch "$(SRC_ROOT)/.flx-extracted"; \
	fi

$(BUILD_BIN): do-prepare
	@printf '[FreeLinX/ports] netcat ready (no compile needed): %s\n' "$(BUILD_BIN)"

do-build: $(BUILD_BIN)

.PHONY: all clean install
all: do-build

install: $(STAGE_FILE)

clean:
	@true

-include $(CURDIR)/distinfo
