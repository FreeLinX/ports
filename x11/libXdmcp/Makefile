# FreeLinX/ports - x11/libXdmcp : X11 DMCP authentication 1.1.5 (upstream port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/lib/libXdmcp-1.1.5.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# The bottom of the X11 dependency line: nothing depends on this except libxcb
# and libXau, and it depends on nothing but the protocol headers.  Produces
# libXdmcp.a.

NAME    := libXdmcp
VERSION := 1.1.5
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= xproto xextproto
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

PREFIX_xproto   = $(FREELINX_BUILD_DIR)/deps/xproto
PREFIX_xextproto = $(FREELINX_BUILD_DIR)/deps/xextproto

# libXdmcp is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/libXdmcp.a $(SRC_TREE)/.libs/libXdmcp.a))
BUILD_BIN   = $(PROJECT_LIB)



# Nothing is forced here about arc4random_buf, and deliberately so.
#
# This port used to pass ac_cv_func_arc4random_buf=yes on the stated grounds that
# "musl does declare arc4random_buf, but only under _BSD_SOURCE or _GNU_SOURCE".
# That is not true of this musl.  1.2.5 has no arc4random at all --
#
#     $ nm -D --defined-only .../lib/libc.so | grep -i arc4random
#     (nothing)
#
# -- so configure answered yes from the forced variable, config.h said
# HAVE_ARC4RANDOM_BUF, and Key.c skipped its own definition and then called a
# function that does not exist:
#
#     Key.c:96:5: error: call to undeclared function 'arc4random_buf'
#
# Left to itself the probe answers no, HAVE_ARC4RANDOM_BUF stays undefined, and
# Key.c compiles the fallback it ships for exactly this case -- a static
# arc4random_buf that calls getentropy(), which musl does have, and falls back
# to srandom/random only if that fails.
PROJECT_CFG_CMDS = \
	./configure \
		--prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME) \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# A library stages itself: there is no single binary for install.mk to copy.
install: all
	$(call flx-stage-pc,libXdmcp,xdmcp)

	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXdmcp/lib" "$(FREELINX_BUILD_DIR)/deps/libXdmcp/include"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libXdmcp/lib/libXdmcp.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libXdmcp/include/"
	@printf '[FreeLinX/ports] staged libXdmcp into %s\n' "$(FREELINX_BUILD_DIR)/deps/libXdmcp"
