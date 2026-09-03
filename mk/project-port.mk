# -*- makefile -*-
# FreeLinX/ports - mk/project-port.mk : generic upstream-project builder.
#
# Complement to mk/base-port.mk (flat NetBSD-src utilities).  This class builds
# REAL upstream projects that ship their own build system (ninja, git, openssh)
# using the FreeLinX clang + LLD + musl toolchain.  It never builds the tool by
# hand and never links host binaries: the project's own build system compiles
# and links against the FreeLinX toolchain via the exported CC/CXX/CFLAGS/LDFLAGS
# environment.  Static linking is driven by the exported flags; if a dependency
# blocks it, the failure surfaces exactly and is never faked into a static win.
#
# A project-port Makefile declares metadata plus:
#   DISTINFO_NAME/ARCHIVE/URL/SHA256  (from its distinfo)
#   SRC_TREE     top source dir after extraction
#                (default $(FREELINX_BUILD_DIR)/work/$(NAME)/$(DISTINFO_NAME))
#   PROJECT_BIN  the final binary path this port delivers (staged to
#                staging/$(INSTALL_RELPATH)).
#   PROJECT_CFG_CMDS   shell commands to fetch+extract+configure (default:
#                      fetch+extract only; most projects override).
#   BUILD_CMDS         shell commands that run the project's own build and
#                      produce $(PROJECT_BIN).
#
# All FreeLinX toolchain flags are exported for the child build to consume.

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

include $(FREELINX_PORTS_ROOT)/mk/port.mk

# --- source layout ----------------------------------------------------------
SRC_DIR?=$(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR?=$(FREELINX_BUILD_DIR)/obj/$(NAME)
DIST_TGZ?=$(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)
DISTINFO_NAME?=$(NAME)
SRC_TREE?=$(SRC_DIR)/$(DISTINFO_NAME)

# --- toolchain as build environment ----------------------------------------
FREELINX_CXX?=$(FREELINX_TOOLCHAIN_BIN)/clang++
FLX_PROJECT_CPPFLAGS?=
FLX_PROJECT_CFLAGS?=-O2
FLX_PROJECT_CXXFLAGS?=-O2 -stdlib=libc++
FLX_PROJECT_LDFLAGS?=
FLX_PROJECT_LIBDIR?=$(FREELINX_TOOLCHAIN_DIR)/lib/$(FREELINX_TRIPLE)
# musl libc++/libc++abi/libunwind are static; expose them for linking C++.
FLX_PROJECT_LIBS?=-L$(FLX_PROJECT_LIBDIR) -lc++ -lc++abi -lunwind

FLX_SHA256_CMD?=sha256sum

export CC:=$(FREELINX_CC)
export CXX:=$(FREELINX_CXX)
export AR:=$(FREELINX_AR)
export LD:=$(FREELINX_LD)
export NM:=$(FREELINX_TOOLCHAIN_BIN)/llvm-nm
export RANLIB:=$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib
export STRIP:=$(FREELINX_TOOLCHAIN_BIN)/llvm-strip
export CFLAGS:=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FLX_PROJECT_STATIC) $(FLX_PROJECT_CFLAGS) $(FLX_PROJECT_CPPFLAGS)
export CXXFLAGS:=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FLX_PROJECT_STATIC) $(FLX_PROJECT_CXXFLAGS) $(FLX_PROJECT_CPPFLAGS)
export CPPFLAGS:=$(FLX_PROJECT_CPPFLAGS)
# --rtlib=compiler-rt: musl has no crtbeginT.o/crtend.o/-lgcc; compiler-rt keeps
# the link GNU-crt free.  -static + -fuse-ld=lld give the static LLVM link.
export LDFLAGS:=$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) --rtlib=compiler-rt -static -fuse-ld=lld $(FLX_PROJECT_LDFLAGS) $(FLX_PROJECT_LIBS)
export LIBS:=

# The delivered artifact; install.mk stages THIS path.
PROJECT_BIN?=
# Recursive expansion is intentional: port Makefiles commonly declare
# PROJECT_BIN after including this framework, so it must be resolved when the
# install rule runs rather than while this file is parsed.
BUILD_BIN=$(PROJECT_BIN)

# --- phases -----------------------------------------------------------------
.PHONY: do-fetch do-extract do-config do-build

do-fetch:
	@set -e; \
	if [ ! -f "$(DIST_TGZ)" ]; then \
		printf '[FreeLinX/ports] fetching %s\n' "$(DISTINFO_URL)"; \
		mkdir -p "$(FREELINX_DIST_DIR)"; \
		(cd "$(FREELINX_DIST_DIR)" && curl -fLSO "$(DISTINFO_URL)"); \
		_c="$$($(FLX_SHA256_CMD) "$(DIST_TGZ)" | awk '{print $$1}')"; \
		if [ -n "$(DISTINFO_SHA256)" ] && [ "$$_c" != "$(DISTINFO_SHA256)" ]; then \
			printf '[FreeLinX/ports][error] sha256 mismatch for %s (got %s)\n' "$(DIST_TGZ)" "$$_c"; \
			exit 1; \
		fi; \
	else \
		printf '[FreeLinX/ports] source present: %s\n' "$(DIST_TGZ)"; \
	fi

do-extract: do-fetch
	@set -e; \
	if [ ! -d "$(SRC_TREE)" ]; then \
		printf '[FreeLinX/ports] extracting %s\n' "$(DISTINFO_ARCHIVE)"; \
		mkdir -p "$(SRC_DIR)"; \
		case "$(DISTINFO_ARCHIVE)" in \
		    *.tar.xz) tar -xJf "$(DIST_TGZ)" -C "$(SRC_DIR)" ;; \
		    *.tar.gz|*.tgz) tar -xzf "$(DIST_TGZ)" -C "$(SRC_DIR)" ;; \
		    *) tar -xf "$(DIST_TGZ)" -C "$(SRC_DIR)" ;; \
		esac; \
		if [ -d patches ]; then \
			printf '[FreeLinX/ports] applying patches for %s\n' "$(NAME)"; \
			for p in patches/*.patch; do \
				if grep -q '^--- ' "$$p" 2>/dev/null; then \
					patch -d "$(SRC_TREE)" -p1 < "$$p"; \
				fi; \
			done; \
		fi; \
	fi

# Configure/host-setup.  Default: fetch+extract only.  Projects with their own
# bootstrap/configure (ninja's configure.py, autotools, cmake) override this
# to run it inside $(SRC_TREE).
PROJECT_CFG_CMDS?=:

do-config: do-extract
	@set -e; \
	cd "$(SRC_TREE)" && $(PROJECT_CFG_CMDS)

# The upstream build; must leave $(PROJECT_BIN) in place.
BUILD_CMDS?=:

do-build: do-config
	@set -e; \
	printf '[FreeLinX/ports] building %s\n' "$(NAME)"; \
	(cd "$(SRC_TREE)" && $(BUILD_CMDS))

# install.mk's `$(STAGE_FILE): $(BUILD_BIN)` drives do-build here.  The
# no-op recipe means the binary itself is produced by do-build (BUILD_CMDS),
# not by a per-object compile-and-link rule in this framework.
$(PROJECT_BIN): do-build
	@test -f "$(PROJECT_BIN)" || { \
		printf '[FreeLinX/ports][error] %s did not produce %s\n' "$(NAME)" "$(PROJECT_BIN)"; \
		exit 1; }
