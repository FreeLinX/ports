# FreeLinX/ports - base/tzdata : the IANA time zone database.
#
# A data port: it builds no binary.  It runs zic(1) over the IANA source and
# stages the compiled zone files, so STAGE_TREE is set and both mk/base-port.mk
# and mk/install.mk treat this as a port whose output is a directory.
#
# -b slim is deliberate.  The default fat format stores the full set of
# transitions for every zone back to the beginning, which is ~1.4 MB; slim
# stores only the transitions still in use, which is ~600 KB for the same 598
# zones.  Nothing in a running system reads the historic data.
#
# Hard links inside the tree are preserved (mk/install.mk copies with cp -a):
# zones that have not changed since 1970 are the same bytes and zic links
# rather than duplicates, so a copy without -a would double the database.

NAME   := tzdata
VERSION:= 2026e
CATEGORY:= base
LICENSE:= public-domain
DEPENDENCIES:=
BUILD_DEPENDENCIES:= zic (from the build host)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /usr

INSTALL_RELPATH:= usr/share/zoneinfo

# STAGE_TREE and the empty BUILD_BIN must be set BEFORE the include: the ifneq
# that selects the data-port branch in mk/base-port.mk is evaluated while that
# file is read, so a STAGE_TREE set afterwards reads as empty and the framework
# goes on to try to link a binary out of an empty source list.  This is the same
# eager-expansion trap the native-port comment above describes.
SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
ZONE_DIR = $(SRC_DIR)/compiled
BUILD_TREE = $(ZONE_DIR)

# The source files zic reads.  The continent files carry the bulk; "backward"
# supplies the aliases people actually type (US/Eastern, and so on) and
# "etcetera" the zoneinfo top-level names.
ZONE_SOURCES = africa antarctica asia australasia europe northamerica \
	southamerica etcetera factory backward

# A data port compiles nothing.  Empty, so the framework does not try to link
# an executable out of an empty source list.
NETBSD_MEMBERS =
PORT_SRCS =
COMPAT_SRCS =
BUILD_BIN =

# The directory mk/install.mk copies to the rootfs.  Before the include, for the
# reason given above.
STAGE_TREE = $(ZONE_DIR)

include ../../mk/base-port.mk

.PHONY: do-prepare
do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_DIR)/.flx-patched" ]; then \
		printf '[FreeLinX/ports] extracting %s into %s\n' "$(DISTINFO_ARCHIVE)" "$(SRC_DIR)"; \
		rm -rf "$(SRC_DIR)"; \
		mkdir -p "$(SRC_DIR)"; \
		tar -xzf "$(DIST_TGZ)" -C "$(SRC_DIR)" --no-same-owner; \
		command -v zic >/dev/null 2>&1 || { \
			printf '[FreeLinX/ports][error] zic is not installed; it compiles the zone files\n' >&2; \
			exit 1; }; \
		touch "$(SRC_DIR)/.flx-patched"; \
	fi

# Named do-build-tree, not do-build: mk/base-port.mk's do-build is the target
# the framework invokes, and a tree port must not claim to produce a binary.
.PHONY: do-build-tree
do-build-tree:
	@set -e; \
	rm -rf "$(ZONE_DIR)"; \
	mkdir -p "$(ZONE_DIR)"; \
	printf '[FreeLinX/ports] zic -b slim -> %s\n' "$(ZONE_DIR)"; \
	cd "$(SRC_DIR)"; \
	zic -b slim -d "$(ZONE_DIR)" $(ZONE_SOURCES); \
	n=$$(find "$(ZONE_DIR)" -type f | wc -l | tr -d ' '); \
	[ "$$n" -gt 100 ] || { \
		printf '[FreeLinX/ports][error] only %s zone files built; that is not a database\n' "$$n" >&2; \
		exit 1; }; \
	for z in UTC Europe/London America/New_York Asia/Tokyo; do \
		[ -f "$(ZONE_DIR)/$$z" ] || { \
			printf '[FreeLinX/ports][error] %s is missing from the built database\n' "$$z" >&2; \
			exit 1; }; \
	done; \
	printf '[FreeLinX/ports] built %s zone files\n' "$$n"

$(BUILD_TREE): do-prepare do-build-tree
	@: # the tree is built by do-build-tree; this target only orders it
