# FreeLinX/ports - sysutils/pfetch : pfetch (Dylan Araps), MIT license.
#
# Metadata:
#   NAME       pfetch
#   CATEGORY   sysutils
#   SOURCE     github.com/dylanaraps/pfetch, master (archived -- frozen).
#              See distinfo.
#   LICENSE    MIT
#   INSTALL    staging/usr/bin/pfetch  ->  FreeLinX/src/rootfs/usr/bin/pfetch
#
# Build model (deliberately different from every other port here):
#   pfetch is a single POSIX sh script. There is nothing to compile and no
#   FreeLinX toolchain (clang/lld/musl sysroot) involved at all -- the
#   script runs directly under FreeLinX's already-ported netbsd-sh. This
#   port therefore does NOT include mk/port.mk (which gates every build on
#   FREELINX_TOOLCHAIN_READY); that gate makes no sense for something that
#   is never compiled. It includes mk/install.mk directly for the shared
#   staging/rootfs-install conventions (STAGE_ROOT, ROOTFS_DIR,
#   check-install-rootfs) without paying for a toolchain check that would
#   always be irrelevant.
#
# Runtime dependencies (all already provided by FreeLinX):
#   POSIX sh (netbsd-sh), uname, cat, and other base utilities pfetch
#   probes for at runtime -- no libc/libdl/libpthread linking involved
#   since there is no compiled binary.
#
# FreeLinX ascii art (deviation from the usual patches/patch-* diff
# convention): pfetch's get_ascii() case statement selects art by matching
# $PF_ASCII/$distro/$os against glob patterns like "([Bb]uildroot*)".
# FreeLinX's /etc/os-release sets PRETTY_NAME="FreeLinX 0.0-dev (bootstrap)",
# which get_os() reads into $distro, so our art needs a
# "([Ff]ree[Ll]in[Xx]*)" branch added to that case statement.
#
# A unified diff patch was deliberately NOT used here: pfetch's heredoc art
# blocks rely on '<<-EOF', which strips only leading TAB characters (not
# spaces) from each line -- and the exact tab depth of the surrounding
# entries can't be reliably confirmed from a terminal-pasted source
# inspection. A byte-for-byte diff context could silently mismatch on
# whitespace. Instead, do-prepare inserts patches/freelinx-ascii-fragment.txt
# via a small awk script anchored on the unique "([Bb]uildroot*)" line --
# this sidesteps the tab/space ambiguity entirely, since the inserted
# fragment supplies its own (zero-indent, therefore unambiguous) heredoc
# body. Case-statement entry order has no effect on matching since no
# existing pattern overlaps "[Ff]ree[Ll]in[Xx]*".
#
# IMPORTANT ORDERING NOTE (bit us once already -- see netbsd-sh's own
# comment on this): NAME/INSTALL_BIN/INSTALL_RELPATH are set BEFORE the
# mk/common.mk + mk/install.mk includes below. install.mk defines the
# staging rule as a literal Makefile TARGET NAME built from these
# variables ($(STAGE_ROOT)/$(INSTALL_RELPATH)); make freezes target names
# at parse time, not at recipe-execution time. Including install.mk before
# these variables are set froze the staging path using whatever NAME
# happened to default to at that point (observed: the build host's
# hostname, from some fallback in common.mk) -- the recipe body itself
# still printed the *correct* NAME/pfetch because recipe bodies are
# expanded lazily at execution time, which is what made the bug
# non-obvious from the log output alone.

NAME    := pfetch
VERSION := master
CATEGORY:= sysutils
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:=

INSTALL_BIN     := pfetch
INSTALL_RELPATH := usr/bin/pfetch

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))../..)

include $(FREELINX_PORTS_ROOT)/mk/common.mk
include $(FREELINX_PORTS_ROOT)/mk/install.mk

SRC_ROOT = $(FREELINX_BUILD_DIR)/work/pfetch/pfetch-master
DIST_TGZ = $(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)

BUILD_BIN = $(SRC_ROOT)/pfetch

# ---------------------------------------------------------------------------
# do-prepare: extract the pristine tarball, mark the delivered script
# executable, and insert the FreeLinX ascii art branch into get_ascii().
# ---------------------------------------------------------------------------
.PHONY: do-prepare do-build

do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_ROOT)/.flx-extracted" ]; then \
		printf '[FreeLinX/ports] extracting %s\n' "$(DISTINFO_ARCHIVE)"; \
		mkdir -p "$(FREELINX_BUILD_DIR)/work/pfetch"; \
		rm -rf "$(SRC_ROOT)"; \
		tar -xzf "$(DIST_TGZ)" -C "$(FREELINX_BUILD_DIR)/work/pfetch"; \
		chmod +x "$(SRC_ROOT)/pfetch"; \
		printf '[FreeLinX/ports] inserting FreeLinX ascii art into get_ascii()\n'; \
		awk ' \
			/\(\[Bb\]uildroot\*\)/ && !done { \
				while ((getline line < "$(CURDIR)/patches/freelinx-ascii-fragment.txt") > 0) print line; \
				close("$(CURDIR)/patches/freelinx-ascii-fragment.txt"); \
				done=1 \
			} \
			{ print } \
		' "$(SRC_ROOT)/pfetch" > "$(SRC_ROOT)/pfetch.tmp"; \
		mv "$(SRC_ROOT)/pfetch.tmp" "$(SRC_ROOT)/pfetch"; \
		chmod +x "$(SRC_ROOT)/pfetch"; \
		touch "$(SRC_ROOT)/.flx-extracted"; \
	fi

$(BUILD_BIN): do-prepare
	@printf '[FreeLinX/ports] pfetch ready (no compile needed): %s\n' "$(BUILD_BIN)"

do-build: $(BUILD_BIN)

.PHONY: all clean install
all: do-build

install: $(STAGE_FILE)

clean:
	@true

-include $(CURDIR)/distinfo
