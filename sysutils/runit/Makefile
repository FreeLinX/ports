# FreeLinX/ports - sysutils/runit : runit 2.2.0 (service supervision, init).
#
# Metadata:
#   NAME       runit
#   VERSION    2.2.0
#   CATEGORY   sysutils
#   SOURCE     smarden.org runit 2.2.0 release tarball (see distinfo)
#   LICENSE    BSD-2-Clause
#   TARGET     x86_64-unknown-linux-musl
#
# Build model (deliberately different from shells/netbsd-sh):
#   runit ships its OWN build system (src/compile, src/load, src/choose) --
#   tiny shell scripts, not bmake/autotools. src/choose does RUNTIME
#   feature-probing: it compiles a small test program with conf-cc, links it
#   with conf-ld, EXECUTES it, and picks a header variant based on what
#   actually happened on this system. Because FreeLinX targets
#   x86_64-linux-musl and this build host is x86_64 Linux (WSL2), those
#   probe binaries genuinely execute here -- same architecture, the kernel
#   does not care about libc. This is the same fact validated by
#   FreeLinX/toolchain's static hello-world test.
#
#   So this port POINTS runit's own scripts at the FreeLinX toolchain
#   (by writing src/conf-cc / src/conf-ld) and lets `package/compile`
#   run unmodified, rather than manually enumerating every .c file the way
#   shells/netbsd-sh does. Hand-reimplementing musl's dirent/flock/sigaction/
#   poll detection ourselves would just be reinventing what choose already
#   does correctly.
#
# musl/static-link note:
#   conf-cc/conf-ld add -rtlib=compiler-rt -unwindlib=none on top of the
#   framework's normal FREELINX_CFLAGS/FREELINX_LDFLAGS. Without these, the
#   clang driver falls back to GCC's libgcc/crtbeginT.o, which the musl
#   sysroot does not provide (the exact failure hit and fixed early in
#   FreeLinX/toolchain bring-up). shells/netbsd-sh avoids this by driving
#   -nostdlib + explicit crt objects itself; here it is simpler to just tell
#   clang to use its own compiler-rt runtime instead.
#
# Suite layout:
#   runit's own Makefile builds ALL its binaries in one pass, sharing
#   intermediate static libs (unix.a, byte.a, time.a) and generated sysdep
#   headers. Splitting this into one-port-per-binary would rebuild those
#   shared pieces N times for no benefit, so this port builds the whole
#   suite and stages the binaries FreeLinX actually needs individually.
#
# Boot-critical: FreeLinX/src/rootfs/init execs /sbin/runsvdir if present.
#   runsvdir -> /sbin/runsvdir   (checked directly by rootfs/init)
#   runsv    -> /sbin/runsv      (execed by runsvdir via $PATH)
#   sv       -> /usr/bin/sv      (admin tool)
#   chpst    -> /usr/bin/chpst   (admin tool)
# Other built binaries (runit, runit-init, svlogd, runsvctrl, runsvstat,
# runsvchdir, svwaitup, svwaitdown, utmpset) are left in the build tree for
# now; add more `install:` lines below when FreeLinX needs them.

NAME   := runit
VERSION:= 2.2.0
CATEGORY:= sysutils
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Satisfy mk/port.mk / mk/install.mk's single-binary defaults with the one
# boot-critical binary; the other three are staged via explicit extra rules
# below (install: accumulates prerequisites across multiple rule lines).
INSTALL_BIN:= runsvdir
INSTALL_RELPATH:= sbin/runsvdir

include ../../mk/port.mk

SRC_ROOT = $(FREELINX_BUILD_DIR)/work/runit/admin/runit-$(VERSION)
SV_DIR   = $(SRC_ROOT)/compile
DIST_TGZ = $(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)

BUILD_BIN = $(SV_DIR)/runsvdir

# ---------------------------------------------------------------------------
# do-prepare: extract the pristine tarball. Idempotent via sentinel, same
# convention as shells/netbsd-sh.
# ---------------------------------------------------------------------------
.PHONY: do-prepare do-configure

do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_ROOT)/.flx-extracted" ]; then \
		printf '[FreeLinX/ports] extracting %s\n' "$(DISTINFO_ARCHIVE)"; \
		mkdir -p "$(FREELINX_BUILD_DIR)/work/runit"; \
		rm -rf "$(SRC_ROOT)"; \
		tar -xzf "$(DIST_TGZ)" -C "$(FREELINX_BUILD_DIR)/work/runit"; \
		touch "$(SRC_ROOT)/.flx-extracted"; \
	fi

# ---------------------------------------------------------------------------
# do-configure: point runit's own build scripts at the FreeLinX toolchain.
# conf-cc/conf-ld are plain one-line files; src/compile and src/load wrap
# whatever is in them with the actual source/object arguments at build time.
# ---------------------------------------------------------------------------
do-configure: do-prepare
	@set -e; \
	printf '[FreeLinX/ports] writing conf-cc / conf-ld for %s\n' "$(FREELINX_TRIPLE)"; \
	printf '%s %s -rtlib=compiler-rt -unwindlib=none -O2\n' \
		"$(CC)" "$(FREELINX_CFLAGS)" > "$(SV_DIR)/conf-cc"; \
	printf '%s %s -rtlib=compiler-rt -unwindlib=none\n' \
		"$(CC)" "$(FREELINX_LDFLAGS)" > "$(SV_DIR)/conf-ld"

# ---------------------------------------------------------------------------
# Build: run runit's own package/compile inside the prepared/configured
# source tree. Any failure surfaces the real compiler/linker error.
# ---------------------------------------------------------------------------
$(BUILD_BIN): do-configure
	@set -e; \
	printf '[FreeLinX/ports] running runit package/compile\n'; \
	cd "$(SRC_ROOT)" && ./package/compile

.PHONY: do-build
do-build: $(BUILD_BIN)
	@printf '[FreeLinX/ports] built: %s\n' "$(BUILD_BIN)"

# ---------------------------------------------------------------------------
# Extra staged binaries beyond the single INSTALL_BIN/INSTALL_RELPATH that
# mk/install.mk's generic rule handles. `install:` here ADDS prerequisites
# to the `install: $(STAGE_FILE)` rule already declared by mk/port.mk; both
# accumulate, since neither has a conflicting recipe body.
# ---------------------------------------------------------------------------
install: $(STAGE_ROOT)/sbin/runsv $(STAGE_ROOT)/usr/bin/sv $(STAGE_ROOT)/usr/bin/chpst

$(STAGE_ROOT)/sbin/runsv: $(SV_DIR)/runsv
	@printf '[FreeLinX/ports] staging runsv -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@

$(STAGE_ROOT)/usr/bin/sv: $(SV_DIR)/sv
	@printf '[FreeLinX/ports] staging sv -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@

$(STAGE_ROOT)/usr/bin/chpst: $(SV_DIR)/chpst
	@printf '[FreeLinX/ports] staging chpst -> %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $< $@

# ---------------------------------------------------------------------------
# Rootfs bridge for the extra binaries beyond INSTALL_BIN/INSTALL_RELPATH.
# `install-rootfs` here ADDS to mk/install.mk's own install-rootfs rule
# (which handles runsvdir via check-install-rootfs + ROOTFS_FILE).
# ---------------------------------------------------------------------------
install-rootfs: $(ROOTFS_DIR)/sbin/runsv $(ROOTFS_DIR)/usr/bin/sv $(ROOTFS_DIR)/usr/bin/chpst

$(ROOTFS_DIR)/sbin/runsv: $(STAGE_ROOT)/sbin/runsv check-install-rootfs
	@printf '[FreeLinX/ports] installing runsv into rootfs %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $(STAGE_ROOT)/sbin/runsv $@

$(ROOTFS_DIR)/usr/bin/sv: $(STAGE_ROOT)/usr/bin/sv check-install-rootfs
	@printf '[FreeLinX/ports] installing sv into rootfs %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $(STAGE_ROOT)/usr/bin/sv $@

$(ROOTFS_DIR)/usr/bin/chpst: $(STAGE_ROOT)/usr/bin/chpst check-install-rootfs
	@printf '[FreeLinX/ports] installing chpst into rootfs %s\n' "$@"
	$(MKDIR) -p $(dir $@)
	$(INSTALL) -m 755 $(STAGE_ROOT)/usr/bin/chpst $@
