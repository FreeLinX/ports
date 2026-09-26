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
#   PORT_COMPAT_SRCS extra FreeLinX compat objects a port needs on top of the
#                    defaults.  Prefer this over COMPAT_SRCS - see below.
#   COMPAT_SRCS      the full set of FreeLinX musl-compat objects.  Defaults to
#                    the list below; a port should add to it with
#                    PORT_COMPAT_SRCS rather than assign this, because assigning
#                    COMPAT_SRCS *replaces* the defaults and every object in
#                    them is then silently absent from the link.
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
#
# Why the split: COMPAT_SRCS?=$(DEFAULT_COMPAT_SRCS) $(PORT_COMPAT_SRCS), and
# PORT_COMPAT_SRCS is read here, before a port can assign it, because port
# Makefiles are written to assign their variables *after* including this file
# and PORT_SRCS already depends on that (a := here would freeze an empty list
# and the port would fail to link with an undefined `main').  PORT_COMPAT_SRCS
# is the one compat variable a port cannot set late and still have take effect,
# so it has to be expanded on every reference rather than captured once.
#
# A port that assigns COMPAT_SRCS outright is not an error - a few genuinely
# want a different set, e.g. to avoid the emalloc/ecalloc/erealloc overlap
# between estdlib.c and the emalloc.c/emalloc-family sources - but it takes
# responsibility for the whole list, including for the default objects it no
# longer pulls in.  netbsd-ping is the worked example of what that costs: it
# overrode COMPAT_SRCS to get getprogname.c and strlcpy.c, which silently
# dropped arc4random.c, and the port then failed at link time with
# `undefined symbol: arc4random' - a symbol nothing in its source ever
# mentioned, coming from a default object it had unknowingly removed.
FLX_DEFAULT_COMPAT_SRCS = \
	$(FLX_COMPAT)/getprogname.c \
	$(FLX_COMPAT)/estrlcpy.c \
	$(FLX_COMPAT)/estdlib.c \
	$(FLX_COMPAT)/arc4random.c \
	$(FLX_COMPAT)/getttynam.c \
	$(FLX_COMPAT)/easprintf.c
PORT_COMPAT_SRCS?=
COMPAT_SRCS?=$(FLX_DEFAULT_COMPAT_SRCS) $(PORT_COMPAT_SRCS)
include $(FREELINX_PORTS_ROOT)/mk/flx-libc.mk
PATCHES?=$(wildcard $(CURDIR)/patches/patch-*)

# FLX_COMPAT, FLX_NBSYS and FLX_NBSYS_FLAGS come from mk/port.mk, which every
# port includes.  They describe the compatibility overlay rather than the
# single-binary build, so they are not defined here: leaving a second copy in
# base-port.mk would let the two drift, and a project port that includes only
# project-port.mk would silently get an empty $(FLX_NBSYS_FLAGS).

# Per-port pre-build generator hook (yacc/lex output, generated tables, ...).
# Runs inside do-prepare after patching; failures abort the build.
FLX_PREGEN?=

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
FLX_CPPFLAGS+=-I$(FLX_COMPAT) -I$(SRC_DIR)/include -include flx_bsd.h $(FLX_NBSYS_FLAGS)

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

# The object name a source file maps to inside OBJ_DIR.  The compile loop in
# the build rule derives the same name in the shell, so the two MUST stay in
# step: the absolute source path with every '/' turned into '_' and the '.c'
# suffix replaced by '.o'.  It is a single definition used both to compile and
# to link, so a rename can never leave the two out of sync.
#   /src/bin/cat/cat.c  ->  $(OBJ_DIR)/_src_bin_cat_cat.o
define flx_obj_of
$(OBJ_DIR)/$(subst /,_,$(basename $(1))).o
endef

# Exactly the objects this port compiles, in ALL_SRCS order: the same
# "absolute path, '/' -> '_', .c -> .o" mapping the compile loop below derives
# in the shell with sed.  The two must agree, because the link line and the
# stale-object prune both work off this list; the recipe below verifies it by
# refusing to link a port whose freshly built objects are not all in it.
#
# This MUST stay lazily expanded (=).  A port assigns PORT_SRCS *after* it
# includes this file, so a simply-expanded (:=) value would freeze an empty
# PORT_SRCS and the link would see only the compat objects - surfacing as
# "undefined symbol: main".
FLX_OBJS        = $(foreach s,$(ALL_SRCS),$(call flx_obj_of,$(s)))

# Link the port's OWN objects and nothing else.  This used to be the
# $(OBJ_DIR)/*.o glob, which silently pulled in every stale object left in the
# directory: an object from an earlier PORT_SRCS set, or one written by the
# pre-2026-09 flat naming scheme (cat.o next to the current mangled name), was
# linked alongside the freshly built one and the link died with
# "duplicate symbol: main" / "duplicate symbol: getprogname".  The compile
# rule below also prunes foreign objects, so a rebuild cannot inherit them.
FLX_LD          = $(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) \
			-nostdlib -L$(FREELINX_SYSROOT)/lib \
			$(FLX_CRT) \
			$(FLX_OBJS) \
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
			tar -xf "$(DIST_TGZ)" -C "$(SRC_DIR)" --strip-components=2 \
				--exclude=CVS --exclude=CVS/Root --exclude=CVS/Entries \
				--no-same-owner $(NETBSD_MEMBERS); \
		fi; \
		for s in $(LOCAL_SRCS); do cp "$$s" "$(SRC_DIR)/"; done; \
		printf '[FreeLinX/ports] applying FreeLinX patches\n'; \
		for p in $(PATCHES); do \
			patch -d "$(SRC_DIR)" -p1 --fuzz=0 < "$$p"; \
		done; \
		if [ -n "$(FLX_PREGEN)" ]; then \
			printf '[FreeLinX/ports] running pre-generate\n'; \
			sh -c '$(FLX_PREGEN)' || exit 2; \
		fi; \
		touch "$(SRC_DIR)/.flx-patched"; \
	fi

# ---------------------------------------------------------------------------
# Build: compile every source (upstream + FreeLinX compat) and statically
# link the binary with clang + LLD + musl sysroot.  Any failure surfaces the
# real compiler/linker error - nothing is faked.
#
# STRIP_CMD runs on the linked binary if set.  A port that commits its own
# binary into the tree - the FreeLinX-native ones, with SRC_DIR = $(CURDIR) and
# BUILD_BIN = $(SRC_DIR)/$(NAME) - dirties the tree on every build, so its
# output has to be reproducible or the tree is permanently dirty.  Unstripped
# clang output also carries the build path, so such a port sets STRIP_CMD to
# keep the committed artifact the same size and content from one build to the
# next.  Off by default: every other port builds into build/ and is staged
# from there, and stripping 293 binaries at once is a separate decision.
# ---------------------------------------------------------------------------
STRIP_CMD ?=

$(BUILD_BIN): do-prepare
	@set -e; \
	mkdir -p "$(OBJ_DIR)"; \
	for s in $(ALL_SRCS); do \
		o="$(OBJ_DIR)/$$(printf '%s' "$$s" | sed 's|/|_|g')"; o="$${o%.c}.o"; \
		printf '[FreeLinX/ports] cc %s\n' "$${s##*/}"; \
		$(CC) $(FREELINX_CFLAGS) $(FLX_CPPFLAGS) -c -o "$$o" "$$s"; \
		case " $(FLX_OBJS) " in \
		*" $$o "*) ;; \
		*) printf '[FreeLinX/ports][error] object name mismatch: %s\n' "$$o"; exit 1 ;; \
		esac; \
	done; \
	for o in "$(OBJ_DIR)"/*.o; do \
		case " $(FLX_OBJS) " in \
		*" $$o "*) ;; \
		*) printf '[FreeLinX/ports] rm stale object %s\n' "$${o##*/}"; rm -f "$$o" ;; \
		esac; \
	done; \
	printf '[FreeLinX/ports] ld %s\n' "$@"; \
	$(FLX_LD) -o "$@"; \
	if [ -n "$(STRIP_CMD)" ]; then printf '[FreeLinX/ports] strip %s\n' "$@"; $(STRIP_CMD) "$@"; fi

.PHONY: do-build
do-build: $(BUILD_BIN)
	@printf '[FreeLinX/ports] built: %s\n' "$(BUILD_BIN)"

# ncurses from the deps tree (host-built): -I include/ncursesw for
# <curses.h>/<term.h>/<term_private.h>, and static link set for CU_* tools.
FLX_NCURSES        := $(FREELINX_PORTS_ROOT)/build/deps/ncurses
FLX_NCURSES_CPPFLAGS = -I$(FLX_NCURSES)/include -I$(FLX_NCURSES)/include/ncursesw
FLX_NCURSES_LDADD    = $(FLX_NCURSES)/lib/libncursesw.a $(FLX_NCURSES)/lib/libtinfo.a
