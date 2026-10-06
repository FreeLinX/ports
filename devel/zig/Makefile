# FreeLinX/ports - devel/zig : Zig 0.17.0 (systems programming language).
#
# Upstream source (BOOTSTRAP tarball, bundles LLVM/Clang/LLD/zlib/zstd/zig):
#   https://ziglang.org/download/0.17.0/zig-bootstrap-0.17.0.tar.xz
# (pinned in distinfo; upstream publishes only a minisig signature, no
# sha256 -- see distinfo for how the recorded sha256 was established.)
# MIT license.
#
# Build model: upstream's hermetic 4-stage bootstrap (see build in SRC_TREE):
#   1. cmake builds host LLVM+Clang+LLD with the SYSTEM clang into out/host.
#   2. cmake builds host zig against stage 1 into out/host/bin/zig.
#   3. host zig cross-compiles zlib/zstd/LLVM+Clang+LLD for
#      x86_64-linux-musl (zig cc -target ... -- its own bundled musl).
#   4. host zig builds zig itself for x86_64-linux-musl into
#      out/zig-x86_64-linux-musl-baseline/ (static musl binary).
# Stages 1-2 are build-host tooling only and are never staged (same split as
# the devel/go port: its bootstrap toolchain stays behind on the build host).
# Only stage 4's output ships: bin/zig plus the lib/ tree zig needs at
# runtime (std lib sources, compiler-rt, bundled musl headers -- `zig
# build-exe` compiles against them, so the binary alone does not function).
#
# Deviations from mk/project-port.mk, all load-bearing and deliberate:
#   * The framework exports CC/CXX/CFLAGS/LDFLAGS (--target/--sysroot,
#     -nostdlib, musl crt objects) for the project's own build to consume.
#     Upstream's script drives its OWN compilers instead -- system clang for
#     stages 1-2, zig-as-cc for stages 3-4 -- so BUILD_CMDS unsets every one
#     of those exports (CC CXX AR LD NM RANLIB STRIP CFLAGS CXXFLAGS CPPFLAGS
#     LDFLAGS LIBS) and sets CC/CXX to the system clang explicitly. Inheriting
#     them would cross the host tools at the FreeLinX sysroot (host tblgen
#     linked -nostdlib against musl crt, or run through zig-cc before zig
#     exists), breaking stage 1 with a link error that names neither flag.
#   * The shipped binary's musl-static property therefore does NOT come from
#     flag inheritance; it comes from zig's own `-target x86_64-linux-musl`
#     hermetic backend, and is verified empirically (file/readelf/check-nognu
#     on the staged output), not assumed from the environment.
#   * CMAKE_GENERATOR=Ninja + CMAKE_BUILD_PARALLEL_LEVEL carry parallelism;
#     the script defaults to make -j1-era serial cmake without them.
#
# Staged layout (go-port precedent): the prefix tree
# out/zig-x86_64-linux-musl-baseline/ ({bin/zig,lib/zig/...}) is staged as
# usr/lib/zig, and usr/bin/zig is a relative symlink into it
# (../lib/zig/bin/zig). The symlink is load-bearing: zig locates its lib/
# tree from its own executable path, so a plain copy at usr/bin/zig would
# lose its standard library.
#
# Heavy build: LLVM+Clang+LLD twice plus zig twice; ~1-3 h on 8 cores.
# Honors FREELINX_JOBS (export FREELINX_JOBS=$(nproc) -- the default is 1).

NAME    := zig
VERSION := 0.17.0
CATEGORY:= devel
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= host clang/cmake/ninja (stages 1-2 only); musl via zig's bundled libc (shipped artifact)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib/zig
# usr/bin/zig is a symlink into the tree (see header); without this,
# install-rootfs copies INSTALL_RELPATH and nothing else, and the staged
# symlink would never reach the rootfs.
INSTALL_EXTRA_RELPATHS:= usr/bin/zig

# Target triple and CPU for the shipped binary. `baseline` = generic x86_64,
# so the staged compiler runs on any machine in the fleet.
ZIG_TARGET := x86_64-linux-musl
ZIG_MCPU   := baseline

# Final prefix produced by stage 4 of the upstream script.
ZIG_OUT = $(SRC_TREE)/out/zig-$(ZIG_TARGET)-$(ZIG_MCPU)

# STAGE_TREE must be assigned BEFORE the include: mk/install.mk selects the
# tree-copy branch with an ifneq evaluated while that file is read (via
# mk/port.mk), so a STAGE_TREE set afterwards reads as empty and the framework
# stages a single file. Recursive (=), so FREELINX_BUILD_DIR resolves later.
# (Same eager-expansion trap devel/go documents.)
STAGE_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/zigroot

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/zig-bootstrap-$(VERSION)
PROJECT_BIN = $(ZIG_OUT)/bin/zig
BUILD_BIN   = $(PROJECT_BIN)

# Host C/C++ for stages 1-2 (build tooling only, never shipped). System
# clang, never the FreeLinX clang: the host tools must run on the build
# host, and the FreeLinX driver targets musl by default.
HOST_CC  := /usr/bin/clang
HOST_CXX := /usr/bin/clang++

# Pruned prefix contents: the binary plus the whole lib/ tree it needs at
# runtime (see header). Everything else the script leaves in out/ (host
# tools, intermediate build dirs) stays behind.
ZIG_KEEP = bin lib
STAGE_STAMP = $(SRC_DIR)/.zigroot-staged

# Upstream's script, verbatim apart from the environment above. CC/CXX name
# the host compiler for the cmake stages; the exports from project-port.mk
# are unset first (see header for why inheriting them breaks stage 1).
BUILD_CMDS = \
	set -e; \
	cd "$(SRC_TREE)" && \
	env -u CC -u CXX -u AR -u LD -u NM -u RANLIB -u STRIP \
	    -u CFLAGS -u CXXFLAGS -u CPPFLAGS -u LDFLAGS -u LIBS \
	    CC="$(HOST_CC)" \
	    CXX="$(HOST_CXX)" \
	    CMAKE_GENERATOR=Ninja \
	    CMAKE_BUILD_PARALLEL_LEVEL="$(FREELINX_JOBS)" \
	    sh ./build "$(ZIG_TARGET)" "$(ZIG_MCPU)" && \
	test -x "$(PROJECT_BIN)" && \
	"$(PROJECT_BIN)" version

# Prepare the pruned prefix tree that mk/install.mk copies to usr/lib/zig.
# do-build is phony, so this recipe runs on every install; the stamp guard
# keeps the multi-GB re-sync to the runs where the compiler was rebuilt.
$(STAGE_TREE): do-build
	@set -e; \
	if [ -d "$(STAGE_TREE)" ] && [ -f "$(STAGE_STAMP)" ] && \
	   [ "$(STAGE_STAMP)" -nt "$(PROJECT_BIN)" ]; then \
		printf '[FreeLinX/ports] zigroot tree current: %s\n' "$(STAGE_TREE)"; \
	else \
		printf '[FreeLinX/ports] staging pruned zig prefix -> %s\n' "$(STAGE_TREE)"; \
		rm -rf "$(STAGE_TREE)"; \
		mkdir -p "$(STAGE_TREE)"; \
		for k in $(ZIG_KEEP); do \
			cp -a "$(ZIG_OUT)/$$k" "$(STAGE_TREE)/"; \
		done; \
		[ -x "$(STAGE_TREE)/bin/zig" ] || { \
			printf '[FreeLinX/ports][error] %s: staged tree has no bin/zig\n' "$(NAME)" >&2; \
			exit 1; }; \
		[ -d "$(STAGE_TREE)/lib/zig" ] || { \
			printf '[FreeLinX/ports][error] %s: staged tree has no lib/zig\n' "$(NAME)" >&2; \
			exit 1; }; \
		touch "$(STAGE_STAMP)"; \
	fi

# usr/bin/zig is a symlink into usr/lib/zig (see header).
# A stage-extra hook, not an install: override, which would lose do-build and
# the generic tree staging (devel/go and devel/nasm use the same hook).
.PHONY: stage-extra
stage-extra:
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@ln -sf ../lib/zig/bin/zig "$(STAGE_ROOT)/usr/bin/zig"
	@printf '[FreeLinX/ports] staged zig symlink\n'
