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

# A port that delivers more than one *file* -- not another name for the same one,
# which is INSTALL_ALIASES -- names the rest here as rootfs-relative paths.
#
# archivers/bzip2 builds two programs, stages both in its own install: recipe,
# and declares only
#
#   INSTALL_RELPATH:= usr/bin/bzip2
#
# install-rootfs copies $(STAGE_FILE) and nothing else, so bzip2recover sat in
# staging/usr/bin for ever.  `git status` on the rootfs never showed it as
# deleted, because it was never there to begin with: a staged file that no
# install rule names is invisible from both ends.
INSTALL_EXTRA_RELPATHS?=

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

# What the generic install targets depend on: the single staged file a binary
# port copies, or nothing at all for a library port that stages itself.
#
# This test cannot depend on BUILD_BIN, and it took a day to work out why.
# GNU make expands a rule's PREREQUISITE LIST when it reads the rule, not when
# it runs it.  install.mk is included from mk/port.mk near the top of a port
# Makefile, long before the port assigns BUILD_BIN further down.  So
#
#     STAGE_DEP = $(if $(strip $(BUILD_BIN)),$(STAGE_FILE))
#
# expanded to the empty string for every port in the tree, always.  The
# dependency was silently dropped, `make install' exited 0 having staged
# nothing, and the next step died with "cannot stat staging/sbin/runsvdir".
#
# The same $(if ...) inside a RECIPE behaves the opposite way -- recipes are
# expanded at execution time, once every variable is bound -- which is why the
# identical test in the rule body worked and the one in the prerequisite list
# did not.  Recipe bodies are not a place to generalise from.
#
# So: LIBRARY_PORT, which a library port declares before the include, exactly
# as it declares NAME and INSTALL_RELPATH.
STAGE_DEP = $(if $(strip $(LIBRARY_PORT)),,$(STAGE_FILE))

# Full path of the same artifact once copied into the FreeLinX/src rootfs.
ROOTFS_FILE=$(ROOTFS_DIR)/$(INSTALL_RELPATH)

# ---------------------------------------------------------------------------
# Rule: stage an already-built binary into the staging overlay. This only copies;
# it never rebuilds, so it never claims success for a compile that did not happen.
#
# A tree port stages $(STAGE_TREE) into $(STAGE_FILE) as a directory, and stages
# nothing at all when it has no tree, so the two cannot both fire for one port.
#
# The prerequisites below are written $$(VAR), not $(VAR), and this is why.
#
# A prerequisite list is expanded when make *reads the rule*.  A recipe is not: it
# is expanded when make runs it, which is why $(BUILD_BIN) inside a recipe works
# even though the port assigns BUILD_BIN fifty lines further down.  A
# prerequisite has no such second chance.
#
# mk/port.mk is included at base-port.mk line 37.  BUILD_BIN? is not assigned
# until line 88, and most ports assign it themselves after their own include, so
# when the rules below are read BUILD_BIN is empty.  The old
#
#     STAGE_FILE: $(BUILD_BIN)
#
# therefore defined a target with *no prerequisites at all*.  Make does not
# complain about that; it runs the staging recipe against a binary that may never
# have been built.
#
# What that costs, measured on base/banner from a clean build/work:
#
#     $ rm -rf build/work/banner build/obj/banner staging/bin/banner
#     $ ./scripts/install.sh -r base/banner
#     install: cannot stat '.../build/work/banner/banner': No such file or directory
#     make: *** [mk/install.mk:134: .../staging/bin/banner] Error 1
#
# `make -p install` shows the target with nothing after the colon:
#
#     /home/.../ports/staging/bin/banner:
#
# Worse, once something has been built, `make install` re-stages whatever is
# lying in build/work/ with no rebuild, so a stale binary is installed as though
# it were current.  426 ports set BUILD_BIN.
#
# .SECONDEXPANSION with $$(VAR) defers the prerequisite until make needs it,
# which is after the whole makefile is read and BUILD_BIN is bound.
# ---------------------------------------------------------------------------
.SECONDEXPANSION:
ifneq ($(strip $(STAGE_TREE)),)

# The tree is copied, not moved: build/ is the port's own output and deleting
# it here would make the next build redo work that has not changed.  cp -a
# because the mode and the hard links inside a zoneinfo or keymap tree are
# part of the data, not packaging noise: zoneinfo hard-links the zones that
# have not changed, and losing the links doubles it.
$(STAGE_FILE): $$(STAGE_TREE) | do-build
	@printf '[FreeLinX/ports] staging %s tree -> %s\n' "$(NAME)" "$@"
	$(MKDIR) -p $(dir $@)
	rm -rf $@
	cp -a $(STAGE_TREE) $@

else

# The rule is defined unconditionally.  install.mk is read *before* the port
# assigns BUILD_BIN lower down in its Makefile, so a parse-time
# `ifneq ($(strip $(BUILD_BIN)),)' guard here tested an empty variable, never
# fired, and no rule existed at all: dbus failed with "No rule to make target
# staging/usr/bin/dbus-daemon, needed by install".
#
# The check therefore belongs in the recipe, where $(BUILD_BIN) is bound by
# then.  A library port (freetype2, libxml2, pcre2) sets PROJECT_LIB instead and
# stages itself in its own install: recipe; STAGE_DEP is empty for it, so this
# recipe is not reached.  If it is reached by accident, say so, rather than
# running `install -m 755  staging/usr/lib` and failing with "missing
# destination file operand".
$(STAGE_FILE): $$(BUILD_BIN) | do-build
	@if [ -z "$(BUILD_BIN)" ]; then \
		printf '[FreeLinX/ports][error] %s: BUILD_BIN is empty; a library port must stage itself in its own install: target\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@printf '[FreeLinX/ports] staging %s -> %s\n' "$(NAME)" "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $(BUILD_BIN) $@

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
install-aliases: $(STAGE_DEP)
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
# Three cases, decided in the recipe rather than by `ifeq` at parse time, for
# the reason the prerequisites above are deferred: STAGE_TREE and BUILD_BIN are
# both usually assigned by the port after its include, so at parse time they read
# empty and the branch is chosen wrong.
#
#   STAGE_TREE set    a tree this port owns, replaced wholesale.  rm -rf first,
#                     because copying over the top of an existing tree leaves
#                     files the new build no longer produces, and a stale zone
#                     left in zoneinfo is a zone the system will read and get
#                     wrong.
#   a directory       a directory the ports share, merged.  Every library port
#                     with INSTALL_RELPATH=usr/lib stages into the same
#                     staging/usr/lib, and that directory is not one port's to
#                     replace: the rootfs usr/lib also holds libc.so,
#                     libunwind.so and dillo/, and the rm -rf above would delete
#                     all of them in order to install one .a file.  This is what
#                     devel/libgc hit, and `install -m 755` on the directory
#                     failed with "install: omitting directory".
#   a file            the ordinary single-binary install.
#
# No `#` comments inside this recipe: a continued recipe is one shell command,
# and a `#` on any of its lines comments out everything after it.
install-rootfs: check-install-rootfs do-build $(STAGE_DEP) install-aliases
	@printf '[FreeLinX/ports] installing %s into rootfs %s\n' "$(NAME)" "$(ROOTFS_FILE)"
	@set -e; \
	$(MKDIR) -p "$(dir $(ROOTFS_FILE))"; \
	if [ -n "$(STAGE_TREE)" ]; then \
		rm -rf "$(ROOTFS_FILE)"; \
		cp -a "$(STAGE_FILE)" "$(ROOTFS_FILE)"; \
	elif [ -d "$(STAGE_FILE)" ]; then \
		cp -a "$(STAGE_FILE)/." "$(ROOTFS_FILE)/"; \
	elif [ -n "$(BUILD_BIN)" ]; then \
		$(INSTALL) -m 755 "$(STAGE_FILE)" "$(ROOTFS_FILE)"; \
	fi
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
	@if [ -n "$(INSTALL_EXTRA_RELPATHS)" ]; then \
	    set -e; \
	    for _r in $(INSTALL_EXTRA_RELPATHS); do \
	        if [ ! -e "$(STAGE_ROOT)/$$_r" ]; then \
	            printf '[FreeLinX/ports][error] %s: INSTALL_EXTRA_RELPATHS names %s, which the port did not stage\n' \
	                "$(NAME)" "$$_r" >&2; \
	            exit 1; \
	        fi; \
	        $(MKDIR) -p "$(ROOTFS_DIR)/$$(dirname $$_r)"; \
	        rm -rf "$(ROOTFS_DIR)/$$_r"; \
	        cp -a "$(STAGE_ROOT)/$$_r" "$(ROOTFS_DIR)/$$_r"; \
	        printf '[FreeLinX/ports] installed %s -> rootfs %s\n' "$$_r" "$(ROOTFS_DIR)/$$_r"; \
	    done; \
	fi
