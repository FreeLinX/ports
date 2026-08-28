# -*- makefile -*-
# FreeLinX/ports - staging and install rules.
#
# Staging model: the <staging> tree is a rootfs-compatible OVERLAY. Each port
# stages its built artifacts at staging/$(INSTALL_RELPATH), which is the exact
# path the file would occupy inside the FreeLinX root filesystem. This lets
# FreeLinX/src consume ports output directly:
#
#   staging/bin/sh   ->   <FREELINX_ROOTFS_DIR>/bin/sh   (default src/rootfs)
#
# Everything (staging root, rootfs template, per-port relpath) is configurable;
# nothing is hard-coded to a developer path, and nothing writes to "/".

# Root of the overlay staging tree (gitignored). Default: <repo>/staging.
STAGE_ROOT?=$(FREELINX_STAGING_ROOT)

# Rootfs-relative destination for a port's staged output. Ports override this
# when they install into a different location, e.g. INSTALL_RELPATH:=bin/sh.
# Recursive (=) so $(INSTALL_BIN) -- defined after include -- resolves lazily.
INSTALL_RELPATH?=bin/$(INSTALL_BIN)

# Default staged binary name within bin/ (a port may override, e.g. INSTALL_BIN=sh).
INSTALL_BIN?=$(NAME)

# The rootfs source template that consumes staged output (default: src/rootfs).
ROOTFS_DIR?=$(FREELINX_ROOTFS_DIR)

# Full path of a port's staged artifact.
STAGE_FILE=$(STAGE_ROOT)/$(INSTALL_RELPATH)

# Full path of the same artifact once copied into the FreeLinX/src rootfs.
ROOTFS_FILE=$(ROOTFS_DIR)/$(INSTALL_RELPATH)

# ---------------------------------------------------------------------------
# Rule: stage an already-built binary into the staging overlay. This only copies;
# it never rebuilds, so it never claims success for a compile that did not happen.
# ---------------------------------------------------------------------------
$(STAGE_FILE): $(BUILD_BIN)
	@printf '[FreeLinX/ports] staging %s -> %s\n' "$(NAME)" "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $(BUILD_BIN) $@

# ---------------------------------------------------------------------------
# Rootfs destination safety. Rootfs installation must always target an explicit,
# configured FreeLinX rootfs directory and never fall back to "/" or an empty
# path. We refuse to proceed if ROOTFS_DIR is unset, "/", or a bare unsafe
# location. This is the guard that makes implicit "/" impossible.
# ---------------------------------------------------------------------------
.PHONY: check-install-rootfs
check-install-rootfs:
	@if [ -z "$(ROOTFS_DIR)" ]; then \
	    printf '[FreeLinX/ports][error] ROOTFS_DIR is empty; refusing rootfs install\n'; \
	    printf '[FreeLinX/ports][error] set FREELINX_ROOTFS_DIR (default: <src>/rootfs), never "/"\n'; \
	    exit 1; \
	fi
	@case "$(ROOTFS_DIR)" in \
	    /*) : ;; \
	    *)  printf '[FreeLinX/ports][error] ROOTFS_DIR is relative: $(ROOTFS_DIR)\n'; \
	        printf '[FreeLinX/ports][error] refusing implicit install destination; use an absolute path\n'; \
	        exit 1 ;; \
	esac
	@case "$(abspath $(ROOTFS_DIR))" in \
	    /) \
	        printf '[FreeLinX/ports][error] refusing to install into the root filesystem "/"\n'; \
	        exit 1 ;; \
	esac
	@printf '[FreeLinX/ports] rootfs destination OK: $(abspath $(ROOTFS_DIR))\n'

# ---------------------------------------------------------------------------
# "Install into rootfs" target: copies a staged artifact into the FreeLinX/src
# rootfs template. This is the ports<->src bridge. It only runs explicitly and
# only after the destination guard above has cleared.
# ---------------------------------------------------------------------------
install-rootfs: check-install-rootfs $(STAGE_FILE)
	@printf '[FreeLinX/ports] installing %s into rootfs %s\n' "$(NAME)" "$(ROOTFS_FILE)"
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
