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

# ---------------------------------------------------------------------------
# Additional names for a port that is one program under several names.
#
# A few NetBSD utilities are a single binary that reads its own argv[0] and
# behaves accordingly: gzip is gunzip, zcat, gzcat and zgrep as well, and sh is
# ash, bash, ksh and rsh by name.  Staging only $(INSTALL_RELPATH) leaves a
# system where the functionality exists but the name does not, and
# "gunzip: not found" from a build script is a real failure even though gzip
# is installed and working.
#
# Format: <alias>=<target>, space separated, both relative to the staged
# binary's own directory.  The target is normally the staged file itself: all
# the aliases point at the one real binary.
#
#   INSTALL_ALIASES := gunzip=gzip zcat=gzip gzcat=gzip
#
# Symbolic links, not copies.  The point is one binary under several names, and
# a copy would be a second program that can drift from the first.
# ---------------------------------------------------------------------------
INSTALL_ALIASES?=

# Rootfs-relative destination for a port's staged output. Ports override this
# when they install into a different location, e.g. INSTALL_RELPATH:=bin/sh.
# Recursive (=) so $(INSTALL_BIN) -- defined after include -- resolves lazily.
INSTALL_RELPATH?=bin/$(INSTALL_BIN)

# Default staged binary name within bin/ (a port may override, e.g. INSTALL_BIN=sh).
INSTALL_BIN?=$(NAME)

# ---------------------------------------------------------------------------
# A port that produces a directory instead of a single file.
#
# Most ports build one binary, so staging is a copy of $(BUILD_BIN) to
# $(STAGE_FILE).  That is wrong for a data port: base/tzdata and
# base/keymaps are trees of hundreds of files, and copying one of them to a
# file path would stage a file where a directory belongs.  A tree port sets
# STAGE_TREE to the directory it built, and the rules below copy the whole
# tree to $(INSTALL_RELPATH) instead.
#
# The opt-in is the existence of STAGE_TREE, not a flag, so a port that does
# not know about this keeps the single-file behaviour with no change.
#
# Recursive (=) because a tree port sets it after including base-port.mk, and
# ':=' would freeze an empty value at parse time.
# ---------------------------------------------------------------------------
STAGE_TREE?=

# The rootfs source template that consumes staged output (default: src/rootfs).
ROOTFS_DIR?=$(FREELINX_ROOTFS_DIR)

# Full path of a port's staged artifact.
STAGE_FILE=$(STAGE_ROOT)/$(INSTALL_RELPATH)

# What the generic install targets depend on.  Empty for a library port: those
# set PROJECT_LIB rather than BUILD_BIN and stage themselves, so there is no
# single staged file for the framework to name.
STAGE_DEP := $(if $(or $(strip $(STAGE_TREE)),$(strip $(BUILD_BIN))),$(STAGE_FILE))

# Full path of the same artifact once copied into the FreeLinX/src rootfs.
ROOTFS_FILE=$(ROOTFS_DIR)/$(INSTALL_RELPATH)

# ---------------------------------------------------------------------------
# Rule: stage an already-built binary into the staging overlay. This only copies;
# it never rebuilds, so it never claims success for a compile that did not happen.
#
# A tree port stages $(STAGE_TREE) into $(STAGE_FILE) as a directory, and stages
# nothing at all when it has no tree, so the two cannot both fire for one port.
# ---------------------------------------------------------------------------
ifneq ($(strip $(STAGE_TREE)),)

# The tree is copied, not moved: build/ is the port's own output and deleting
# it here would make the next build redo work that has not changed.  cp -a
# because the mode and the hard links inside a zoneinfo or keymap tree are
# part of the data, not packaging noise: zoneinfo hard-links the zones that
# have not changed, and losing the links doubles it.
$(STAGE_FILE): $(STAGE_TREE)
	@printf '[FreeLinX/ports] staging %s tree -> %s\n' "$(NAME)" "$@"
	$(MKDIR) -p $(dir $@)
	rm -rf $@
	cp -a $(STAGE_TREE) $@

else

# Only when the port has a single binary to stage.  A library port sets
# PROJECT_LIB (freetype2, libxml2, pcre2) instead of BUILD_BIN and stages
# itself in its own install: recipe.  Without this guard the rule below still
# exists for it, with an empty $(BUILD_BIN), so make runs
#
#     install -m 755  staging/usr/lib
#
# and fails with "missing destination file operand".  INSTALL_RELPATH is still
# set on those ports, so STAGE_FILE is a real path and the rule looks valid.
ifneq ($(strip $(BUILD_BIN)),)

$(STAGE_FILE): $(BUILD_BIN)
	@printf '[FreeLinX/ports] staging %s -> %s\n' "$(NAME)" "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $(BUILD_BIN) $@

endif

endif

# ---------------------------------------------------------------------------
# Stage the port's aliases as symlinks beside the staged binary.
#
# One recipe looping over the list, rather than a generated rule per alias.
# A rule per alias means a $(foreach) that $(eval)s inside, and make's own
# whitespace handling in that position is easy to get subtly wrong - an alias
# list that silently produces no rules is a staged system quietly missing the
# names it was supposed to have.  The parse-time check below is there so that a
# malformed entry is reported loudly instead, and there is only one place where
# the list is interpreted.
# ---------------------------------------------------------------------------
$(foreach _flx_al,$(INSTALL_ALIASES),\
  $(if $(filter-out 2,$(words $(subst =, ,$(_flx_al)))),\
    $(warning INSTALL_ALIASES entry is not <alias>=<target>: $(_flx_al)),))

.PHONY: install-aliases
install-aliases: $(if $(strip $(BUILD_BIN)),$(STAGE_FILE))
	@if [ -n "$(INSTALL_ALIASES)" ]; then \
	    set -e; \
	    d="$(dir $(STAGE_FILE))"; \
	    for _a in $(INSTALL_ALIASES); do \
	        case $$_a in *=*) : ;; \
	            *) printf '[FreeLinX/ports][error] %s: INSTALL_ALIASES entry is not <alias>=<target>: %s\n' "$(NAME)" "$$_a" >&2; exit 1 ;; \
	        esac; \
	        _n=$${_a%%=*}; _t=$${_a#*=}; \
	        [ -n "$$_n" ] && [ -n "$$_t" ] || { \
	            printf '[FreeLinX/ports][error] %s: INSTALL_ALIASES entry has an empty alias or target: %s\n' "$(NAME)" "$$_a" >&2; exit 1; }; \
	        printf '[FreeLinX/ports] staging %s: %s -> %s\n' "$(NAME)" "$$_n" "$$_t"; \
	        rm -f "$$d$$_n"; \
	        $(LN) -sf "$$_t" "$$d$$_n"; \
	    done; \
	fi

	@:

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
# STAGE_DEP is empty for a library port, which stages itself and has no single
# staged file for the generic rules to depend on.
install-rootfs: check-install-rootfs $(STAGE_DEP) install-aliases
	@printf '[FreeLinX/ports] installing %s into rootfs %s\n' "$(NAME)" "$(ROOTFS_FILE)"
	$(MKDIR) -p $(dir $(ROOTFS_FILE))
ifeq ($(strip $(STAGE_TREE)),)
	$(if $(strip $(BUILD_BIN)),$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE),:)
else
	# A tree, so it is replaced as a directory.  rm -rf first: copying over
	# the top of an existing tree leaves files that the new build no longer
	# produces, and a stale zone left in zoneinfo is a zone the system will
	# happily read and get wrong.
	rm -rf $(ROOTFS_FILE)
	cp -a $(STAGE_FILE) $(ROOTFS_FILE)
endif
	@if [ -n "$(INSTALL_ALIASES)" ]; then \
	    set -e; \
	    d="$(dir $(ROOTFS_FILE))"; \
	    for _a in $(INSTALL_ALIASES); do \
	        _n=$${_a%%=*}; _t=$${_a#*=}; \
	        printf '[FreeLinX/ports] installing %s alias %s -> %s into rootfs\n' \
	            "$(NAME)" "$$_n" "$$_t"; \
	        rm -f "$$d$$_n"; \
	        $(LN) -s "$$_t" "$$d$$_n"; \
	    done; \
	fi
