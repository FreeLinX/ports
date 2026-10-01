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

# ac_cv_func_arc4random_buf=yes is required, and is not a guess.
#
# musl does declare arc4random_buf, but only under _BSD_SOURCE or _GNU_SOURCE.
# configure probes for it with a default-flags program that calls it, so the
# declaration is not visible, the probe fails, HAVE_ARC4RANDOM_BUF stays unset,
# and Key.c falls back to defining its own:
#
#     static void arc4random_buf (void *auth, int len)
#
# Key.c does define _GNU_SOURCE, so by then musl's declaration is visible too
# and the build dies with
#
#     Key.c:77:1: error: static declaration of 'arc4random_buf' follows
#     non-static declaration
#
# Telling configure the truth is the whole fix: it then uses libc's.
PKG_PATH = $(FREELINX_BUILD_DIR)/deps/xproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xextproto/lib/pkgconfig

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		ac_cv_func_arc4random_buf=yes \
		PKG_CONFIG_PATH="$(PKG_PATH)" \
		PKG_CONFIG="/usr/bin/pkg-config" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# A library stages itself: there is no single binary for install.mk to copy.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXdmcp/lib" "$(FREELINX_BUILD_DIR)/deps/libXdmcp/include"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libXdmcp/lib/libXdmcp.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libXdmcp/include/"
	@printf '[FreeLinX/ports] staged libXdmcp into %s\n' "$(FREELINX_BUILD_DIR)/deps/libXdmcp"
