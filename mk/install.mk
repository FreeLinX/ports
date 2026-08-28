# -*- makefile -*-
# FreeLinX/ports - staging and install rules.
#
# A successful port produces a staged binary under (default)
#   staging/$(FREELINX_TRIPLE)/bin/<NAME>
# which FreeLinX/src later consumes into its rootfs as /bin/<NAME>.
#
# The staging tree is version-controlled-ignored (see .gitignore). Nothing in
# this framework writes to the live system "/".

# Per-port staging prefix (kept separate so multi-arch builds don't collide).
STAGE_PREFIX?=$(FREELINX_STAGING_DIR)/$(FREELINX_TRIPLE)/$(CATEGORY)/$(NAME)

# Where a port's staged binary lands. Defaults to the port name; override
# via INSTALL_BIN (e.g. the "sh" shell installs to staging/bin/sh).
INSTALL_BIN?=$(NAME)

# The destination inside the FreeLinX/src rootfs once "install" is run.
# Default mirrors src/rootfs/bin.
INSTALL_ROOTFS_DIR?=$(FREELINX_ROOTFS_BIN)

# ---------------------------------------------------------------------------
# Rules. Port Makefiles typically define a build recipe and then:
#   install: $(STAGE_PREFIX)/bin/$(INSTALL_BIN)
#
# The staging install rule just copies an already-built binary; it does not
# rebuild, so it never claims success for a compile that did not happen.
# ---------------------------------------------------------------------------
$(STAGE_PREFIX)/bin/$(INSTALL_BIN): $(BUILD_BIN)
	@printf '[FreeLinX/ports] staging %s -> %s\n' "$(NAME)" "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $(BUILD_BIN) $@

# ---------------------------------------------------------------------------
# "Install into the rootfs" target: copies a staged binary into the
# FreeLinX/src rootfs (default src/rootfs/bin). This is the eventual bridge;
# it is only run explicitly, never silently.
# ---------------------------------------------------------------------------
install-rootfs: $(STAGE_PREFIX)/bin/$(INSTALL_BIN)
	@printf '[FreeLinX/ports] installing %s into rootfs %s\n' "$(NAME)" "$(INSTALL_ROOTFS_DIR)"
	$(MKDIR) -p $(INSTALL_ROOTFS_DIR)
	$(INSTALL) -m 755 $(STAGE_PREFIX)/bin/$(INSTALL_BIN) $(INSTALL_ROOTFS_DIR)/$(INSTALL_BIN)
