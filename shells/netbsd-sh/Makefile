# FreeLinX/ports - shells/netbsd-sh : NetBSD `/bin/sh` (Almquist shell, ash).
#
# This is the FIRST real FreeLinX port. It builds the actual NetBSD 10.1 sh
# from the NetBSD base src set with the FreeLinX toolchain (clang + LLD + musl
# sysroot). It is NOT an invented shell, a fake binary, or a host /bin/sh.
#
# Metadata:
#   NAME       netbsd-sh
#   VERSION    10.1        (NetBSD release the source is drawn from)
#   CATEGORY   shells
#   SOURCE     NetBSD 10.1 src set (external/bsd/bin/sh == bin/sh; see distinfo)
#   LICENSE    BSD-2-Clause
#   TARGET     x86_64-unknown-linux-musl
#   INSTALL    staging/bin/sh  ->  FreeLinX/src/rootfs/bin/sh
#
# Build model:
#   Host tools (awk/sed/sh) run NetBSD's own source generators (mkbuiltins,
#   mktokens, mknodes.sh, mknodenames.sh, mkoptions.sh, mkinit.sh) - exactly
#   as NetBSD cross-builds do (USETOOLS/HOST_PROG).  Those generators produce
#   C and header files; they are NOT part of the delivered binary.
#
#   The C is then compiled and statically linked ONLY with the FreeLinX
#   toolchain:  clang --target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT)
#   -fuse-ld=lld -static.  No host compiler, host binutils, host /bin/sh
#   binary, or system clang is used for the build itself.
#
# musl compatibility (all small, isolated; see patches/README):
#   - compat/sys/cdefs.h  FreeLinX stand-in for BSD <sys/cdefs.h> macros musl
#                         does not provide (plus the timespeccmp() helper).
#   - compat/getprogname.c  musl lacks getprogname(3)/setprogname(3).
#   - patch-myhistedit-nohistory  build without libedit/history (-DSMALL).
#   - patch-main-fmaxfd-linux     musl has no fcntl(F_MAXFD).
#   - patch-cd-getcwd-linux       use libc getcwd() instead of forking pwd.

NAME   := netbsd-sh
VERSION:= 10.1
CATEGORY:= shells
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Install destination, staged as bin/sh -> staging/bin/sh -> src/rootfs/bin/sh.
# NOTE: INSTALL_BIN / INSTALL_RELPATH must be set BEFORE including mk/port.mk:
# make freezes rule-target names at parse time, so a value set after the include
# would not reach the staging rule.
INSTALL_BIN:= sh
INSTALL_RELPATH:= bin/sh

include ../../mk/port.mk

# ---------------------------------------------------------------------------
# Source layout in the build work tree.
#
# The NetBSD 10.1 src set keeps `/bin/sh` at bin/sh (it sat at
# external/bsd/bin/sh in earlier releases).  do-prepare extracts the pristine
# tree, normalizes usr/src up to the NetBSD src root, and makes
# external/bsd/bin/sh resolve to bin/sh so both paths agree.
# ---------------------------------------------------------------------------
SRC_ROOT = $(FREELINX_BUILD_DIR)/work/netbsd-sh
SH_SRC   = $(SRC_ROOT)/external/bsd/bin/sh

DIST_TGZ = $(FREELINX_DIST_DIR)/$(DISTINFO_ARCHIVE)

# Upstream sh sources (NetBSD bin/sh/Makefile SHSRCS).  histedit.c is kept
# even though FreeLinX builds without history (-DSMALL): under SMALL it
# supplies the histcmd()/inputrc() error stubs.
SH_SRCS = alias.c arith_token.c arithmetic.c cd.c error.c eval.c exec.c \
	expand.c histedit.c input.c jobs.c mail.c main.c memalloc.c \
	miscbltin.c mystring.c options.c parser.c redir.c show.c trap.c \
	output.c var.c syntax.c

# Sources the sh build pulls in from elsewhere in the NetBSD tree (upstream
# .PATH: bltin/ + bin/test + bin/kill + usr.bin/printf).
EXTRA_SH_SRCS = $(SH_SRC)/bltin/echo.c
EXTRA_ROOT_SRCS = $(SRC_ROOT)/bin/test/test.c \
	$(SRC_ROOT)/bin/kill/kill.c \
	$(SRC_ROOT)/usr.bin/printf/printf.c

# FreeLinX musl-compat layer shipped with this port.  getprogname.c provides
# getprogname/setprogname/setproctitle; setmode.c is NetBSD's own libc
# lib/libc/gen/setmode.c (drop-in, used by the sh umask builtin's symbolic
# mode); signalname.c provides signalname(3)/signalnumber(3) for the trap
# builtin.  All compiled into the sh link set.
FLX_COMPAT  = $(CURDIR)/compat
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/setmode.c \
	$(FLX_COMPAT)/signalname.c

# Files NetBSD's generators emit (bin/sh/Makefile GENSRCS/GENHDRS) plus the
# object dir that holds them and the compiled objects.
GEN_SRCS = builtins.c init.c nodes.c
GEN_HDRS = builtins.h nodes.h token.h nodenames.h optinit.h
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)

# Built binary (source of the staging copy; mk/install.mk depends on it).
BUILD_BIN = $(SRC_ROOT)/sh

# Every .c compiled for this port: upstream + generated + FreeLinX compat.
# Basenames are all unique, so each object is OBJ_DIR/<basename>.o.
ALL_SRCS = $(addprefix $(SH_SRC)/,$(SH_SRCS)) $(EXTRA_SH_SRCS) \
	$(EXTRA_ROOT_SRCS) $(addprefix $(OBJ_DIR)/,$(GEN_SRCS)) $(COMPAT_SRCS)

# Compile flags for every translation unit.  -DSMALL is NetBSD's own "no
# history/editline" configuration; the bltin sources want -DSHELL.  Includes
# are ordered so the FreeLinX compat dir claims <sys/cdefs.h> before anything
# else could.
FLX_CPPFLAGS = -DSHELL -DSMALL -DUSE_LRAND48 -DHAVE_VASPRINTF=1 \
	-I$(FLX_COMPAT) -I$(OBJ_DIR) -I$(SH_SRC)

# NetBSD runs its generator scripts as host programs (USETOOLS): plain POSIX
# sh + awk/sed.  They are build-time scaffolding, not the delivered binary.
# compat/netbsd-mktemp emulates NetBSD `mktemp -t` for GNU coreutils.
HOST_SH = /bin/sh
MKTEMP   = $(FLX_COMPAT)/netbsd-mktemp

# Static-link set as clang's driver provides it on a musl sysroot: crt1.o +
# crti.o at the front, crtn.o at the very end, -lc in between, and clang's
# compiler-rt builtins in place of the libgcc a GCC toolchain would add.
# The musl sysroot has no crtbegin*/crtend*/libgcc, so the link is driven
# explicitly with -nostdlib rather than trusting the driver defaults.
#
# compiler-rt lives in the *clang resource* tree (lib/clang/<ver>/lib/), not
# the sysroot, under the compiler's normalized target triple (for a
# x86_64-linux-musl sysroot that is x86_64-unknown-linux-musl).  It is looked
# up through `clang -print-resource-dir` so the path follows the toolchain
# wherever it is installed.
FLX_CRT         = $(FREELINX_SYSROOT)/lib/crt1.o $(FREELINX_SYSROOT)/lib/crti.o
FLX_CRT_END     = $(FREELINX_SYSROOT)/lib/crtn.o
FLX_RTDIR       := $(shell $(CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -print-resource-dir 2>/dev/null)
FLX_COMPILER_RT = $(filter %-musl/libclang_rt.builtins.a,$(wildcard $(FLX_RTDIR)/lib/*/libclang_rt.builtins.a))
FLX_LD         = $(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) \
			-nostdlib -L$(FREELINX_SYSROOT)/lib \
			$(FLX_CRT) \
			$(OBJ_DIR)/*.o \
			-lc $(FLX_COMPILER_RT) \
			$(FLX_CRT_END)

PATCHES = $(wildcard $(CURDIR)/patches/patch-*)

# ---------------------------------------------------------------------------
# do-prepare: create a pristine work tree from dist/ and apply FreeLinX
# patches.  A .flx-patched sentinel makes repeated builds idempotent; delete
# build/work/netbsd-sh (or run a full clean) to force a re-extract.
# ---------------------------------------------------------------------------
.PHONY: do-prepare do-generate

do-prepare:
	@set -e; \
	if [ ! -f "$(SRC_ROOT)/.flx-patched" ]; then \
		printf '[FreeLinX/ports] extracting NetBSD 10.1 src into %s\n' "$(SRC_ROOT)"; \
		rm -rf "$(SRC_ROOT)"; \
		mkdir -p "$(SRC_ROOT)"; \
		tar -xzf "$(DIST_TGZ)" -C "$(SRC_ROOT)"; \
		mv "$(SRC_ROOT)/usr/src/"* "$(SRC_ROOT)/"; \
		rmdir "$(SRC_ROOT)/usr/src"; \
		mkdir -p "$(SRC_ROOT)/external/bsd"; \
		ln -s ../../bin "$(SRC_ROOT)/external/bsd/bin"; \
		printf '[FreeLinX/ports] applying FreeLinX patches\n'; \
		for p in $(PATCHES); do \
			patch -d "$(SRC_ROOT)" -p1 < "$$p"; \
		done; \
		touch "$(SRC_ROOT)/.flx-patched"; \
	fi

# ---------------------------------------------------------------------------
# do-generate: run the NetBSD source generators into a fresh OBJ_DIR.
# ---------------------------------------------------------------------------
do-generate:
	@set -e; \
	rm -rf "$(OBJ_DIR)"; \
	mkdir -p "$(OBJ_DIR)"; \
	printf '[FreeLinX/ports] generating sh sources (builtins/nodes/tokens/options/init)\n'; \
	(cd "$(OBJ_DIR)" && AWK=awk SED=sed MKTEMP="$(MKTEMP)" "$(HOST_SH)" "$(SH_SRC)/mktokens"); \
	AWK=awk SED=sed "$(HOST_SH)" "$(SH_SRC)/mkbuiltins" "$(SH_SRC)/shell.h" "$(SH_SRC)/builtins.def" "$(OBJ_DIR)"; \
	(cd "$(OBJ_DIR)" && AWK=awk SED=sed "$(HOST_SH)" "$(SH_SRC)/mkinit.sh" \
		$(addprefix $(SH_SRC)/,$(SH_SRCS)) $(EXTRA_SH_SRCS) $(EXTRA_ROOT_SRCS)); \
	AWK=awk SED=sed "$(HOST_SH)" "$(SH_SRC)/mknodes.sh" \
		"$(SH_SRC)/nodetypes" "$(SH_SRC)/nodes.c.pat" "$(OBJ_DIR)"; \
	AWK=awk SED=sed "$(HOST_SH)" "$(SH_SRC)/mknodenames.sh" "$(OBJ_DIR)/nodes.h" \
		> "$(OBJ_DIR)/nodenames.h"; \
	AWK=awk SED=sed MKTEMP="$(MKTEMP)" "$(HOST_SH)" "$(SH_SRC)/mkoptions.sh" \
		"$(SH_SRC)/option.list" optinit.h "$(OBJ_DIR)"

# ---------------------------------------------------------------------------
# Build: compile every source (including generated ones and the FreeLinX
# compat shim) and statically link sh with clang + LLD + musl sysroot.  Any
# failure surfaces the real compiler/linker error - nothing is faked.
# ---------------------------------------------------------------------------
$(BUILD_BIN): do-prepare do-generate
	@set -e; \
	for s in $(ALL_SRCS); do \
		o="$(OBJ_DIR)/$${s##*/}"; o="$${o%.c}.o"; \
		printf '[FreeLinX/ports] cc %s\n' "$${s##*/}"; \
		$(CC) $(FREELINX_CFLAGS) $(FLX_CPPFLAGS) -c -o "$$o" "$$s"; \
	done; \
	printf '[FreeLinX/ports] ld %s\n' "$@"; \
	$(FLX_LD) -o "$@"

.PHONY: do-build
do-build: $(BUILD_BIN)
	@printf '[FreeLinX/ports] built: %s\n' "$(BUILD_BIN)"
