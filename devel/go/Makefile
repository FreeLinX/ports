# FreeLinX/ports - devel/go : Go 1.27.1 (Go programming language toolchain).
#
# Upstream source: https://go.dev/dl/go1.27.1.src.tar.gz (pinned in distinfo).
# BSD-3-Clause license. The source tarball unpacks to a top directory named
# `go` (not `go1.27.1`), hence the explicit SRC_TREE below.
#
# Bootstrap (host-only build tooling, NOT shipped): this machine has no Go
# installed, and Go >= 1.24 refuses to build without a working Go >= 1.24.6
# already present (cmd/dist aborts otherwise). So PROJECT_CFG_CMDS downloads
# the prebuilt go1.26.8 linux-amd64 toolchain
# (https://go.dev/dl/go1.26.8.linux-amd64.tar.gz) into the port work dir
# build/work/go/bootstrap/ and points GOROOT_BOOTSTRAP at it. It never enters
# dist/ (only the SOURCE tarball lives in distinfo) and never enters staging:
# it only compiles the new toolchain, then stays behind on the build host.
# The host is x86_64 glibc Linux, so the prebuilt bootstrap runs as-is.
#
# Build: cd src && GOROOT_BOOTSTRAP=<bootstrap> GOTOOLCHAIN=local
# CGO_ENABLED=0 GOOS=linux GOARCH=amd64 CC=<FreeLinX clang> ./make.bash.
# GOTOOLCHAIN=local keeps the bootstrap `go` from trying to download a newer
# toolchain mid-build; GOCACHE points into the work dir so the host's
# ~/.cache is left alone. CC names the FreeLinX clang for the record, but
# with CGO_ENABLED=0 nothing C is ever compiled: the go/gofmt binaries and
# the compiler tools under pkg/tool are pure Go plus assembly.
#
# CGO_ENABLED=0 is also the baked-in default of the resulting toolchain, and
# that is deliberate: it matches this tree's static philosophy. Every binary
# the toolchain produces is a fully static ELF with no libc dependency at
# all, so there is no glibc/musl mixing question. cgo against musl (a CC
# wrapper carrying --target/--sysroot) comes later; until then building with
# CGO_ENABLED=1 fails loudly instead of silently linking the host libc.
#
# Staged layout: the built GOROOT (pruned to bin, pkg, src, VERSION,
# LICENSE -- api, doc, misc, test and the markdown files are dropped) is
# staged as the tree usr/lib/go, and usr/bin/go + usr/bin/gofmt are relative
# symlinks into it (../lib/go/bin/go). The symlinks are load-bearing, not
# decoration: the go binary locates GOROOT from its own executable path, so
# it must really live at $GOROOT/bin/go. A plain copy at usr/bin/go would
# lose its GOROOT. GOROOT src/ ships because `go build` compiles the stdlib
# from source; that plus pkg/tool is why the tree is large (~0.5 GB).
#
# Verify: staging/usr/bin/go version; a hello-world `go build` producing a
# static ELF with an empty .comment (no GCC markers); gofmt reformats.

NAME    := go
VERSION := 1.27.1
CATEGORY:= devel
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib/go
# usr/bin/go and usr/bin/gofmt are symlinks into the tree (see above); without
# this, install-rootfs copies INSTALL_RELPATH and nothing else, and the staged
# symlinks would never reach the rootfs.
INSTALL_EXTRA_RELPATHS:= usr/bin/go usr/bin/gofmt

# STAGE_TREE must be assigned BEFORE the include: mk/install.mk selects the
# tree-copy branch with an ifneq evaluated while that file is read (via
# mk/port.mk), so a STAGE_TREE set afterwards reads as empty and the framework
# stages a single file. Recursive (=), so FREELINX_BUILD_DIR resolves later.
# (Same eager-expansion trap base/tzdata documents for mk/base-port.mk.)
STAGE_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/goroot

include ../../mk/project-port.mk

# The source tarball's top directory is `go`, not `go1.27.1`.
SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/go
PROJECT_BIN = $(SRC_TREE)/bin/go
BUILD_BIN   = $(PROJECT_BIN)

# Host-only bootstrap toolchain (see header). Lives in the work dir, never in
# dist/ or staging. The bootstrap tarball's top directory is also `go`, so it
# extracts into bootstrap/ rather than beside the source tree.
BOOTSTRAP_VERSION := 1.26.8
BOOTSTRAP_ARCHIVE := go$(BOOTSTRAP_VERSION).linux-amd64.tar.gz
BOOTSTRAP_URL := https://go.dev/dl/$(BOOTSTRAP_ARCHIVE)
BOOTSTRAP_SHA256 := d0f743b33e8d8945e6b1f432edd15785c70507121d6e2a723b21285eddf8b57b
BOOTSTRAP_STAGE = $(SRC_DIR)/bootstrap
BOOTSTRAP_TGZ = $(BOOTSTRAP_STAGE)/$(BOOTSTRAP_ARCHIVE)
BOOTSTRAP_ROOT = $(BOOTSTRAP_STAGE)/go

# Pruned GOROOT contents (see header) and the stamp that makes re-staging
# incremental: the tree is only re-synced when the built go binary is newer.
GOROOT_KEEP = bin pkg src VERSION LICENSE
STAGE_STAMP = $(SRC_DIR)/.goroot-staged

PROJECT_CFG_CMDS = \
	set -e; \
	mkdir -p "$(BOOTSTRAP_STAGE)"; \
	if [ ! -x "$(BOOTSTRAP_ROOT)/bin/go" ]; then \
		printf '[FreeLinX/ports] fetching bootstrap %s\n' "$(BOOTSTRAP_URL)"; \
		rm -f "$(BOOTSTRAP_TGZ)"; \
		(cd "$(BOOTSTRAP_STAGE)" && curl -fL -o "$(BOOTSTRAP_ARCHIVE)" "$(BOOTSTRAP_URL)"); \
		_c=$$(sha256sum "$(BOOTSTRAP_TGZ)" | awk '{print $$1}'); \
		if [ "$$_c" != "$(BOOTSTRAP_SHA256)" ]; then \
			printf '[FreeLinX/ports][error] bootstrap sha256 mismatch (got %s)\n' "$$_c" >&2; \
			rm -f "$(BOOTSTRAP_TGZ)"; \
			exit 1; \
		fi; \
		tar -xzf "$(BOOTSTRAP_TGZ)" -C "$(BOOTSTRAP_STAGE)"; \
	fi; \
	"$(BOOTSTRAP_ROOT)/bin/go" version

BUILD_CMDS = \
	set -e; \
	cd "$(SRC_TREE)/src" && \
	GOROOT_BOOTSTRAP="$(BOOTSTRAP_ROOT)" \
	GOTOOLCHAIN=local \
	CGO_ENABLED=0 \
	GOOS=linux GOARCH=amd64 \
	CC="$(FREELINX_CC)" \
	GOCACHE="$(SRC_DIR)/gocache" \
	./make.bash && \
	test -x "$(PROJECT_BIN)" && \
	"$(PROJECT_BIN)" version

# Prepare the pruned GOROOT tree that mk/install.mk copies to usr/lib/go.
# do-build is phony, so this recipe runs on every install; the stamp guard
# keeps the ~0.5 GB re-sync to the runs where the toolchain was rebuilt.
$(STAGE_TREE): do-build
	@set -e; \
	if [ -d "$(STAGE_TREE)" ] && [ -f "$(STAGE_STAMP)" ] && \
	   [ "$(STAGE_STAMP)" -nt "$(PROJECT_BIN)" ]; then \
		printf '[FreeLinX/ports] goroot tree current: %s\n' "$(STAGE_TREE)"; \
	else \
		printf '[FreeLinX/ports] staging pruned GOROOT -> %s\n' "$(STAGE_TREE)"; \
		rm -rf "$(STAGE_TREE)"; \
		mkdir -p "$(STAGE_TREE)"; \
		for k in $(GOROOT_KEEP); do \
			cp -a "$(SRC_TREE)/$$k" "$(STAGE_TREE)/"; \
		done; \
		[ -x "$(STAGE_TREE)/bin/go" ] || { \
			printf '[FreeLinX/ports][error] %s: staged tree has no bin/go\n' "$(NAME)" >&2; \
			exit 1; }; \
		[ -x "$(STAGE_TREE)/bin/gofmt" ] || { \
			printf '[FreeLinX/ports][error] %s: staged tree has no bin/gofmt\n' "$(NAME)" >&2; \
			exit 1; }; \
		[ -d "$(STAGE_TREE)/pkg/tool" ] || { \
			printf '[FreeLinX/ports][error] %s: staged tree has no pkg/tool\n' "$(NAME)" >&2; \
			exit 1; }; \
		[ -d "$(STAGE_TREE)/src/runtime" ] || { \
			printf '[FreeLinX/ports][error] %s: staged tree has no src/runtime\n' "$(NAME)" >&2; \
			exit 1; }; \
		[ -f "$(STAGE_TREE)/VERSION" ] || { \
			printf '[FreeLinX/ports][error] %s: staged tree has no VERSION\n' "$(NAME)" >&2; \
			exit 1; }; \
		touch "$(STAGE_STAMP)"; \
	fi

# usr/bin/go and usr/bin/gofmt are symlinks into usr/lib/go (see header).
# A stage-extra hook, not an install: override, which would lose do-build and
# the generic tree staging (devel/nasm and devel/lua use the same hook).
.PHONY: stage-extra
stage-extra:
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@ln -sf ../lib/go/bin/go "$(STAGE_ROOT)/usr/bin/go"
	@ln -sf ../lib/go/bin/gofmt "$(STAGE_ROOT)/usr/bin/gofmt"
	@printf '[FreeLinX/ports] staged go symlinks\n'
