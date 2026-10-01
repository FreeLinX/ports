# FreeLinX/ports - www/w3m : w3m 0.5.3-git20230121.
#
# Real upstream source:
#   https://github.com/tats/w3m/archive/refs/tags/v0.5.3%2Bgit20230121.tar.gz
#
# Text-mode web browser with SSL.  This is the last of the three committed
# prebuilts in the rootfs: usr/bin/w3m carried GCC: (GNU) 16.1.1.
#
# The distinfo points at the GitHub tag archive rather than at SourceForge's
# w3m-0.5.3.tar.gz.  SourceForge's /download URL answers with a 16-byte redirect
# stub, and the sha256 the old distinfo recorded for the release tarball,
# e994d263f2fd2c22febfbe45103526e00145a7674a0fda79c822b97c2770a9e3, matches
# nothing that URL serves.  The tag archive is the same 0.5.3 tree plus the
# post-release fixes, and its own sha256 is what distinfo records.
#
# w3m license.  Autotools build.

NAME    := w3m
VERSION := 0.5.3-git20230121
CATEGORY:= www
LICENSE := w3m
DEPENDENCIES:= libgc openssl ncurses
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN := w3m
INSTALL_RELPATH := usr/bin/w3m

# w3m links wc and its own bookmarker/helper binaries, and linkwc.c needs the C++
# compiler.  CXX=no makes configure stop with
#
#   configure: error: C++ compiler cannot create executables
FLX_PROJECT_CXX := yes

DEPS = $(FREELINX_BUILD_DIR)/deps

# bdwgc's version, for the host build of mktable below.  devel/libgc builds
# 8.2.8 from the same tarball, and the two have to agree: mktable is compiled
# against the source tree's include/ and the host libgc.a has to be that tree's.
# Checked, not assumed -- see the guard in PROJECT_CFG_CMDS.
LIBGC_VERSION := 8.2.8

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/w3m-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/w3m

OPENSSL_PREFIX = $(DEPS)/openssl
NCURSES_PREFIX = $(DEPS)/ncurses
LIBGC_PREFIX = $(FREELINX_PORTS_ROOT)/staging/usr

# Where the host-side build goes: a mktable that can run on this machine.
#
# w3m ships no generated tables in any tarball -- not w3m-0.5.3.tar.gz, not the
# git tag archive, not Debian's orig tarball -- and Makefile.in generates them
# with a program that has to execute:
#
#   mktable$(EXT): mktable.o dummy.o Str.o hash.o myctype.o
#   functable.c: funcname.tab mktable$(EXT)
#           sort funcname.tab | $(AWK) -f $(top_srcdir)/functable.awk > functable.tab
#           ./mktable$(EXT) 100 functable.tab > $@
#   tagtable.c: tagtable.tab mktable$(EXT)
#           ./mktable$(EXT) 100 tagtable.tab > $@
#   entity.h: entity.tab mktable$(EXT)
#
# so `make w3m` builds mktable with $(CC) -- the cross compiler -- and then runs
# it.  It cannot run it.  The tables are therefore generated before make looks at
# them, by a mktable built here for the host, and make finds them newer than
# their prerequisites and leaves them alone.
#
# mktable.c includes <gc.h> and calls GC_INIT and GC_malloc, so a libgc for the
# host comes first.  It is the same source devel/libgc builds for the target,
# copied rather than configured out of the source directory: automake's own
# preamble refuses a VPATH configure when the source directory already holds a
# config.status, and devel/libgc leaves one behind, so the whole output of
# building it out of tree is
#
#   configure: error: source directory already configured; run "make distclean"
#   there first
#
# clang, like everything else here; the old port used CC=/usr/bin/cc for exactly
# this stage, which put a GCC-built object and a GCC-built libgcc.a into the
# build.
NATIVE = $(FREELINX_BUILD_DIR)/work/$(NAME)/native
LIBGC_SRC = $(FREELINX_BUILD_DIR)/work/libgc/bdwgc-$(LIBGC_VERSION)

# --disable-image and --disable-xface: an image viewer needs the image protocol
# and an X11 display, and this is the base image's text browser.
#
# --with-termlib=ncursestw, not ncursesw, because base/ncurses builds reentrant
# and reentrant ncurses names its library libncursestw.a.  A terminfo library is
# not optional: terms.c declares the termcap entry points itself and calls
# tgetent, tgetstr, tputs and tgoto.  AC_W3M_TERMLIB runs AC_CHECK_LIB over the
# list, so the name is the argument and there is no -l flag to add by hand.
#
# ac_cv_func_setpgrp_void=yes: setpgrp() returns void on Linux and int on the
# BSDs this configure was written against, and w3m does not test for it.
W3M_CFG_FLAGS = \
	--prefix=/usr \
	--host=$(FREELINX_TRIPLE) \
	--disable-image \
	--disable-xface \
	--with-ssl=$(OPENSSL_PREFIX) \
	--with-gc=$(LIBGC_PREFIX) \
	--with-termlib=ncursestw \
	ac_cv_func_setpgrp_void=yes

# -nostdlib and musl's own crt objects, named explicitly, in the order mk's own
# link line uses: crt1.o crti.o, then -lc, then crtn.o.  -lc is not optional --
# -nostdlib also means no implicit libraries, so the very first link test in
# configure stops with
#
#   ld.lld: error: undefined symbol: __libc_start_main
#   >>> referenced by crt1.c
#
# before it has looked at anything of w3m's.  w3m's link rule is
#
#   $(CC) $(CFLAGS) -o $(TARGET) $(ALLOBJS) $(LDFLAGS) $(LIBS) $(EXT_LIBS) $(W3M_LIBS)
#
# so LDFLAGS is where they belong, and without -nostdlib clang links the host's
# GCC crtbeginT.o and the binary comes out with
#
#   GCC: (GNU) 16.1.1 20260625
#
# in .comment beside its own clang string.  That was the last GCC marker in the
# rootfs and it arrived through the startup files, not through a library.
W3M_LDFLAGS = \
	$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) \
	-nostdlib --rtlib=compiler-rt -static -fuse-ld=lld \
	$(FLX_CRT) -lc $(FLX_CRT_END) \
	-L$(OPENSSL_PREFIX)/lib -L$(LIBGC_PREFIX)/lib -L$(NCURSES_PREFIX)/lib

# The -Wno- spellings here are clang's.  The old port passed
# -Wno-deprecated-non-prototype as well, which is GCC's spelling of
# -Wdeprecated-non-prototype; clang does not know it and says
# unknown warning option on every file.
# -std=gnu17, and not clang's default, which is gnu23.  In C23 a declaration with
# an empty parameter list means (void), and w3m has
#
#   linein.c:53: void (*InputKeymap[32]) () = { ... };
#   linein.c:211:         (*InputKeymap[(int)c]) (c);
#
# A key handler table declared with no prototype and called with arguments.  That
# is an unprototyped declaration, which C17 still allows and C23 does not, so the
# build stops at
#
#   linein.c:211:27: error: too many arguments to function call, expected 0, have 1
#
# C17 is the last standard where the declaration means what w3m's author meant.
# C89 would also work and is arguably closer to the source, but musl's headers are
# not C89-clean, so gnu17 is the lowest that builds.
# The toolchain flags come first and are not optional.  w3m's configure puts
# whatever CFLAGS it was given into the generated Makefile verbatim, and this
# port's earlier version of them did not carry --target and --sysroot, so
# every file compiled against the *host's* glibc headers:
#
#   # 285 "/usr/include/bits/floatn-common.h" 3 4
#   extern int fscanf (...) __asm__ ("" "__isoc23_fscanf") ...
#
# and the link then stopped with
#
#   ld.lld: error: undefined symbol: __isoc23_strtol
#   ld.lld: error: undefined symbol: __isoc23_sscanf
#   ld.lld: error: undefined symbol: __isoc23_strtoll
#
# because musl 1.2.5 has no __isoc23_* -- it has no isoc23 in any of its
# headers, and libc.a defines none:
#
#   $ nm --defined-only x86_64-linux-musl/lib/libc.a | grep -c __isoc23
#   0
#
# A cross build whose CFLAGS omit the target triple is not a cross build.
W3M_CFLAGS = \
	$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) \
	-std=gnu17 -O2 -I. -I.. -I./libwc \
	-I$(OPENSSL_PREFIX)/include \
	-I$(LIBGC_PREFIX)/include \
	-Wno-pointer-sign -Wno-int-conversion \
	-Wno-implicit-function-declaration -Wno-implicit-int \
	-Wno-incompatible-pointer-types -Wno-format-security

W3M_CXXFLAGS = \
	$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) \
	-O2 -stdlib=libc++

PROJECT_CFG_CMDS = \
	cd "$(SRC_TREE)" && \
	test -f "$(LIBGC_SRC)/include/gc.h" || { \
		printf '[FreeLinX/ports][error] w3m: %s is missing; build devel/libgc first\n' "$(LIBGC_SRC)" >&2; \
		exit 1; \
	}; \
	grep -q '^VERSION *:= *$(LIBGC_VERSION)$$' "$(FREELINX_PORTS_ROOT)/devel/libgc/Makefile" || { \
		printf '[FreeLinX/ports][error] w3m: LIBGC_VERSION is $(LIBGC_VERSION) and devel/libgc says something else\n' >&2; \
		exit 1; \
	}; \
	autoconf -o configure configure.ac && \
	./configure $(W3M_CFG_FLAGS) \
		CC="$(FREELINX_CC)" \
		CXX="$(FREELINX_CXX)" \
		CFLAGS="$(W3M_CFLAGS)" \
		CXXFLAGS="$(W3M_CXXFLAGS)" \
		LDFLAGS="$(W3M_LDFLAGS)"

# Three steps.
#
# 1. A host libgc and a host mktable, in $(NATIVE).  The four sources are the
#    four Makefile.in names for mktable's objects, minus dummy.o, which is the
#    empty library stub upstream links in and has nothing to compile.  config.h
#    has to exist for them, which is why this runs after the configure above.
#
# 2. The tables, with make's own recipes, run with -o mktable.
#
#    -o is what makes this possible.  Makefile.in's rule is
#
#      mktable$(EXT): mktable.o dummy.o Str.o hash.o myctype.o
#
#    and dummy.o is unbuildable: there is no dummy.c in the tree, no rule for it
#    in the generated Makefile, and nothing else produces it.  It is a name
#    automake reserves for the object of an empty convenience library, left in
#    the rule after the library went away.  So asking make for functable.c
#    without -o makes it try to build mktable, which cannot succeed, and fails
#    with
#
#      make[1]: *** [Makefile:182: mktable] Error 1
#
#    -o mktable tells make the target is up to date, which is true: it was just
#    built, by the one compiler whose output this machine can run.
#
#    The Makefile's funcname.tab rule calls awk, sort and cmp by bare name
#    through PATH rather than through $(AWK), so there is nothing to override
#    there either.  Those are host programs on this host and were on every host
#    that has built w3m; none of them produces an object file.
#
# 3. w3m itself, cross.
BUILD_CMDS = \
	if [ ! -f "$(NATIVE)/bdwgc/Makefile" ]; then \
		mkdir -p "$(NATIVE)"; \
		cp -a "$(LIBGC_SRC)" "$(NATIVE)/bdwgc"; \
		rm -f "$(NATIVE)/bdwgc/config.cache"; \
		cd "$(NATIVE)/bdwgc" && \
		./configure \
			--disable-shared --enable-static \
			--disable-cplusplus \
			--disable-java-support --disable-gcj-support \
			CC="$(FREELINX_CC)" CFLAGS="-O2" >/dev/null; \
	fi && \
	$(MAKE) -C "$(NATIVE)/bdwgc" -j"$(FREELINX_JOBS)" >/dev/null && \
	cd "$(SRC_TREE)" && \
	for f in mktable Str hash myctype; do \
		$(FREELINX_CC) $(FLX_NATIVE_CFLAGS) -DHAVE_CONFIG_H \
			-I. -I./libwc -I"$(LIBGC_SRC)/include" \
			-c -o "$(NATIVE)/$$f.o" "$$f.c" || exit 1; \
	done && \
	$(FREELINX_CC) $(FLX_NATIVE_CFLAGS) -o "$(NATIVE)/mktable" \
		"$(NATIVE)/mktable.o" "$(NATIVE)/Str.o" "$(NATIVE)/hash.o" \
		"$(NATIVE)/myctype.o" \
		-L"$(NATIVE)/bdwgc/.libs" -lgc -lm && \
	cp "$(NATIVE)/mktable" "$(SRC_TREE)/mktable" && \
	touch "$(SRC_TREE)/mktable" && \
	$(MAKE) -j"$(FREELINX_JOBS)" -o mktable \
		funcname.tab funcname.c funcname1.h funcname2.h \
		functable.c tagtable.c entity.h && \
	$(MAKE) -j"$(FREELINX_JOBS)" w3m

FLX_NATIVE_CFLAGS = -O2