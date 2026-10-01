# FreeLinX/ports - x11/fontconfig : font configuration 2.15.0 (upstream port).
#
# Real upstream source:
#   https://distfiles.macports.org/fontconfig/fontconfig-2.15.0.tar.xz
#
# fontconfig is not an xorg package, which is why it is not on www.xorg at all:
# /pub/individual/ has no fontconfig-*.  This is the upstream release tarball,
# taken from the Gentoo/MacPorts distfiles mirror.
#
# Produces libfontconfig.a.  libXft links against it and lemonbar needs libXft.
#
# DEPS_ONLY and LIBRARY_PORT both: a .a is build-time only, and a runtime font
# cache is machine-specific data, not something to bake into a rootfs template.
#
# ---- STATUS: builds up to the final link, then stops on zlib ----------------
#
# The three ac_cv_* answers below are load-bearing, and each one was arrived at
# by reading config.log rather than by guessing:
#
# fontconfig detects expat with ac_fn_c_check_func on XML_SetDoctypeDeclHandler.
# That emits `char XML_SetDoctypeDeclHandler ();`, links -lexpat, and answers
# "no" here -- so configure dies with
#
#     *** expat is required. or try to use --enable-libxml2
#
# on a system where the header compiles, the archive links by hand, and
# pkg-config reports expat 2.6.4.  Answering the probe directly leaves the rest
# of configure's real probing intact.  Setting LIBS=-lexpat instead is worse:
# LIBS rides on every link test configure runs, and the first one then fails
# with "C compiler cannot create executables", which says even less about why.
#
# ac_cv_header_xmlparse_h is for the same reason: the probe wants xmlparse.h,
# the expat 1.x name that 2.x dropped.  devel/expat stages a one-line shim.
#
# Two library-name facts that cost most of the time here:
#
#   -l must be in LIBS, not LDFLAGS.  A link line is
# CC $(LDFLAGS) $(OBJECTS) $(LIBS), so a library named in LDFLAGS is listed
# before the objects it is meant to resolve and is silently dropped.  fc-cache
# wants -lz and died on "undefined symbol: inflateInit2_" with -L set correctly.
#
#   LDFLAGS= passed to ./configure *replaces* the framework's exported LDFLAGS
# rather than adding to it, so FLX_PROJECT_LDFLAGS never arrives.  Every -L has
# to be repeated in the inline LDFLAGS, which is why the -L list appears twice
# in this file.
#
# -ljson-c because fontconfig 2.15 moved its cache format to json-c.
# devel/json-c is the port for it.

# The -L flags are repeated in the inline LDFLAGS below on purpose.  Passing
# LDFLAGS= to ./configure *replaces* the framework's exported LDFLAGS rather
# than adding to it, so FLX_PROJECT_LDFLAGS never reaches this build and the
# link fails at "unable to find library -lfreetype" with the flags visible
# in the Makefile the whole time.
NAME    := fontconfig
VERSION := 2.15.0
CATEGORY:= x11
LICENSE := MIT-like (fontconfig)
DEPENDENCIES:= freetype2 expat zlib json-c
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), freetype2 (devel/freetype2)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

# freetype2's install: target stages into the overlay, not build/deps, so this
# points at where the file actually is: -L.../deps/freetype2/lib holds nothing
# and the link fails with "unable to find library -lfreetype".
PREFIX_freetype2 = $(FREELINX_STAGING_ROOT)/usr
PREFIX_expat = $(FREELINX_BUILD_DIR)/deps/expat
PREFIX_zlib = $(FREELINX_BUILD_DIR)/deps/zlib
PREFIX_jsonc = $(FREELINX_BUILD_DIR)/deps/json-c

# fontconfig is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_freetype2)/include -I$(PREFIX_expat)/include -I$(PREFIX_zlib)/include -I$(PREFIX_jsonc)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_freetype2)/lib -L$(PREFIX_expat)/lib -L$(PREFIX_zlib)/lib -L$(PREFIX_jsonc)/lib

include ../../mk/project-port.mk

# The .la is NOT a candidate.  libtool writes a 1 KB ASCII "libtool library
# file" next to the real archive, and listing it first means PROJECT_LIB picks
# that text file -- which then gets staged as libfontconfig.a and fails the
# first port that links it.  Only the real archive counts.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libfontconfig.a $(SRC_TREE)/src/libfontconfig.a))
BUILD_BIN   = $(PROJECT_LIB)



PROJECT_CFG_CMDS = \
	./configure \
		--prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME) \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--without-docbook \
		--without-iconv \
		--disable-docs \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		ac_cv_header_expat_h=yes \
		ac_cv_header_xmlparse_h=yes \
		ac_cv_func_XML_SetDoctypeDeclHandler=yes \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_freetype2)/include -I$(PREFIX_expat)/include -I$(PREFIX_zlib)/include -I$(PREFIX_jsonc)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_freetype2)/lib -L$(PREFIX_expat)/lib -L$(PREFIX_zlib)/lib -L$(PREFIX_jsonc)/lib"

# CPPFLAGS carries json-c's include directory for the test/ subdirectory.
# fontconfig's own configure records JSON_CFLAGS but its test/Makefile.am never
# refers to them, and the test includes <json.h> directly, so with json-c
# support on -- which fontconfig 2.15 always has -- that is where the build stops:
#
#   test-conf.c:28:10: fatal error: 'json.h' file not found
#
# The tests are noinst_PROGRAMS, so plain make builds them.  The path is
# include/json: json-c's headers are all under that directory.  The include
# itself is patches/0001, because fontconfig 2.15.0 asks for a <json.h> that
# json-c does not ship.
# Only the library, and that is deliberate rather than a shortcut.
#
# A plain make walks SUBDIRS into test/, whose programs are noinst_PROGRAMS and
# are therefore built by "all".  test/test-conf.c reaches into fontconfig's own
# structs and calls lh_table_head() on a json-c linkhash, and json-c's
# *installed* linkhash.h is the opaque one: the declarations moved into
# json_object_private.h, which is not public.  So the test does not compile
# against a correctly installed json-c:
#
#   test-conf.c:50:5: error: call to undeclared function 'lh_table_head'
#
# Building src/ alone gives the libfontconfig.a this port stages and installs,
# and leaves a broken upstream test where it belongs -- out of the product.
#
# CPPFLAGS is not needed here any more: devel/json-c stages json.h, which is
# what test-conf.c asks for, so the patches/0001 include swap is belt to that.
# SUBDIRS with test/ dropped, deliberately rather than as a shortcut.
#
# A plain make walks SUBDIRS into test/, whose programs are noinst_PROGRAMS and
# so are built by "all".  test/test-conf.c reaches into fontconfig's own structs
# and calls lh_table_head() on a json-c linkhash, and json-c's *installed*
# linkhash.h is the opaque one -- the declarations moved into
# json_object_private.h, which is not public.  So the test does not compile
# against a correctly installed json-c:
#
#   test-conf.c:50:5: error: call to undeclared function 'lh_table_head'
#
# Everything else is built, including the generated headers src/ needs --
# fcalias.h among them, which is why "make -C src" on its own stops at
#   ./fcint.h:123:10: fatal error: 'fcalias.h' file not found
BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" \
		SUBDIRS="fontconfig fc-case fc-lang src fc-cache fc-cat fc-conflist \
			fc-list fc-match fc-pattern fc-query fc-scan fc-validate conf.d its po"

install: all
	$(call flx-stage-pc,fontconfig,fontconfig)

	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/fontconfig/lib" "$(FREELINX_BUILD_DIR)/deps/fontconfig/include"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/fontconfig/lib/libfontconfig.a"
	# The headers go into include/fontconfig/, not include/ flat.  Consumers
	# include them as <fontconfig/fontconfig.h>, so a flat copy produces a tree
	# where every header is present and libXft still stops at "fatal error:
	# 'fontconfig/fontconfig.h' file not found".
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/fontconfig/include/fontconfig"
	@cp $(SRC_TREE)/fontconfig/*.h "$(FREELINX_BUILD_DIR)/deps/fontconfig/include/fontconfig/"
	@printf '[FreeLinX/ports] staged fontconfig into %s\n' "$(FREELINX_BUILD_DIR)/deps/fontconfig"
