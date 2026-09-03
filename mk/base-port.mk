# -*- makefile -*-
# FreeLinX/ports - mk/base-port.mk : shared recipe for the base utilities.
#
# Every base port (cat, cp, echo, ls, mkdir, mv, rm) builds the ACTUAL NetBSD
# 10.1 source of that utility with the FreeLinX toolchain (clang + LLD + musl
# sysroot), statically linked, exactly like the netbsd-sh port.  It is NOT an
# invented tool, a fake binary, or a host coreutils program: the C is the
# pristine src set pulled from dist/, the result is compiled and linked only
# with the FreeLinX toolchain.
#
# A port Makefile declares its metadata plus:
#   SRC_DIR          extraction root (default build/work/$(NAME))
#   NETBSD_MEMBERS   tar members of $(DISTINFO_ARCHIVE) to extract; paths are
#                    the src set's own "usr/src/..." (the leading two
#                    components are stripped, so usr/src/bin/cp/cp.c lands at
#                    $(SRC_DIR)/bin/cp/cp.c and usr/src/include/fts.h at
#                    $(SRC_DIR)/include/fts.h).
#   PORT_SRCS        upstream .c files to compile, relative to $(SRC_DIR).
#   COMPAT_SRCS      FreeLinX musl-compat objects (default:
#                    base/compat/getprogname.c; mkdir adds setmode.c).
#   FLX_CPPFLAGS     extra -D/-I flags (e.g. -DSMALL, -DHAVE_NBTOOL_CONFIG_H).
#   BUILD_BIN        the delivered statically-linked binary; mk/install.mk
#                    stages it at staging/$(INSTALL_RELPATH).
#
# Everything else - selective extraction, patch application, the static-link
# ceremony (crt + -lc + clang compiler-rt, driven with -nostdlib because the
# musl sysroot has no crtbegin/crtend/libgcc) - is shared here so there is a
# single source of truth.

FREELINX_PORTS_ROOT:=$(abspath $(dir $(lastword $(MAKEFILE_LIST)))..)

include $(FREELINX_PORTS_ROOT)/mk/port.mk

SRC_DIR?=$(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR?=$(FREELINX_BUILD_DIR)/obj/$(NAME)
DIST_TGZ?=$(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)
FLX_COMPAT?=$(FREELINX_PORTS_ROOT)/base/compat
COMPAT_SRCS?=$(FLX_COMPAT)/getprogname.c
PATCHES?=$(wildcard $(CURDIR)/patches/patch-*)

NETBSD_MEMBERS?=
# Source files maintained directly by a FreeLinX port.  This is for a small
# Linux-native backend when an upstream BSD kernel ABI has no Linux analogue.
# They are copied into SRC_DIR before the normal compile/link path.
LOCAL_SRCS?=
PORT_SRCS?=
BUILD_BIN?=$(SRC_DIR)/$(NAME)

ALL_SRCS = $(addprefix $(SRC_DIR)/,$(PORT_SRCS)) $(COMPAT_SRCS)

# Default flags for every base utility: the shared FreeLinX compat layer leads
# the include path, and extracted upstream headers (src set include/) are
# reachable as their own <fts.h>/<vis.h>.  A port extends FLX_CPPFLAGS with
# the configuration it needs (-DSMALL, -DHAVE_NBTOOL_CONFIG_H=1, ...).
FLX_CPPFLAGS?=-I$(FLX_COMPAT) -I$(SRC_DIR)/include

# Static-link set, identical to shells/netbsd-sh: crt1.o + crti.o at the
# front, crtn.o at the very end, -lc in between, and clang compiler-rt in
# place of the libgcc a GCC toolchain would add.  compiler-rt lives in the
# clang resource tree (lib/clang/<ver>/lib/) under the compiler's normalized
# target triple; it is looked up through `clang -print-resource-dir` so the
# path follows the toolchain wherever it is installed.
FLX_CRT         = $(FREELINX_SYSROOT)/lib/crt1.o $(FREELINX_SYSROOT)/lib/crti.o
FLX_CRT_END     = $(FREELINX_SYSROOT)/lib/crtn.o
FLX_RTDIR       := $(shell $(CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -print-resource-dir 2>/dev/null)
FLX_COMPILER_RT = $(filter %-musl/libclang_rt.builtins.a,$(wildcard $(FLX_RTDIR)/lib/*/libclang_rt.builtins.a))
# FLX_LDADD: extra statically-linked archives/libs for a port (e.g. the
# FreeLinX openssl port's libcrypto.a for dc's BIGNUM math).  Placed between
# the port objects and libc, so a static archive's own libc references still
# resolve.  A port sets it in its Makefile (never hard-coded paths).
FLX_LDADD	 =
FLX_LD          = $(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) \
			-nostdlib -L$(FREELINX_SYSROOT)/lib \
			$(FLX_CRT) \
			$(OBJ_DIR)/*.o \
			$(FLX_LDADD) \
			-lc $(FLX_COMPILER_RT) \
			$(FLX_CRT_END)

# ---------------------------------------------------------------------------
# do-prepare: extract only the members this utility needs from dist/ and
# apply the FreeLinX patches.  A .flx-patched sentinel keeps repeated builds
# idempotent; delete build/work/$(NAME) to force a re-extract.
# ---------------------------------------------------------------------------
.PHONY: do-prepare

do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_DIR)/.flx-patched" ]; then \
		printf '[FreeLinX/ports] extracting %s members into %s\n' "$(DISTINFO_ARCHIVE)" "$(SRC_DIR)"; \
		rm -rf "$(SRC_DIR)"; \
		mkdir -p "$(SRC_DIR)"; \
		if [ -n "$(NETBSD_MEMBERS)" ]; then \
			tar -xf "$(DIST_TGZ)" -C "$(SRC_DIR)" --strip-components=2 $(NETBSD_MEMBERS); \
		fi; \
		for s in $(LOCAL_SRCS); do cp "$$s" "$(SRC_DIR)/"; done; \
		printf '[FreeLinX/ports] applying FreeLinX patches\n'; \
		for p in $(PATCHES); do \
			patch -d "$(SRC_DIR)" -p1 < "$$p"; \
		done; \
		touch "$(SRC_DIR)/.flx-patched"; \
	fi

# ---------------------------------------------------------------------------
# Build: compile every source (upstream + FreeLinX compat) and statically
# link the binary with clang + LLD + musl sysroot.  Any failure surfaces the
# real compiler/linker error - nothing is faked.
# ---------------------------------------------------------------------------
$(BUILD_BIN): do-prepare
	@set -e; \
	mkdir -p "$(OBJ_DIR)"; \
	for s in $(ALL_SRCS); do \
		o="$(OBJ_DIR)/$$(printf '%s' "$$s" | sed 's|/|_|g')"; o="$${o%.c}.o"; \
		printf '[FreeLinX/ports] cc %s\n' "$${s##*/}"; \
		$(CC) $(FREELINX_CFLAGS) $(FLX_CPPFLAGS) -c -o "$$o" "$$s"; \
	done; \
	printf '[FreeLinX/ports] ld %s\n' "$@"; \
	$(FLX_LD) -o "$@"

.PHONY: do-build
do-build: $(BUILD_BIN)
	@printf '[FreeLinX/ports] built: %s\n' "$(BUILD_BIN)"
