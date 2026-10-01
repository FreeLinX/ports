# FreeLinX/ports - x11/libX11 : libX11 1.8.13 (upstream project port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/lib/libX11-1.8.13.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# A library port.  It produces libX11.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := libX11
VERSION := 1.8.12
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libxcb libXau libXdmcp xproto xextproto kbproto inputproto xtrans xorgproto
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
# DEPS_ONLY: this port stages nothing into the overlay or the rootfs.
# It exists to put headers and a .a in build/deps for the next port
# in the chain.  install.sh needs to be told, or it expects a staged
# binary that a library never has.
DEPS_ONLY := yes
# LIBRARY_PORT: no single staged binary, so the framework's staging
# rules are skipped and the port stages its own .a and headers.
# Declared BEFORE the include, because make reads prerequisite lists
# at parse time -- see the STAGE_DEP note in mk/install.mk.
LIBRARY_PORT := yes
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

# Dependency prefixes, from the ports that install into build/deps.
PREFIX_libxcb = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libXau = $(FREELINX_BUILD_DIR)/deps/libXau
PREFIX_libXdmcp = $(FREELINX_BUILD_DIR)/deps/libXdmcp
PREFIX_xproto = $(FREELINX_BUILD_DIR)/deps/xproto
PREFIX_xextproto = $(FREELINX_BUILD_DIR)/deps/xextproto
PREFIX_kbproto = $(FREELINX_BUILD_DIR)/deps/kbproto
PREFIX_inputproto = $(FREELINX_BUILD_DIR)/deps/inputproto
PREFIX_xtrans  = $(FREELINX_BUILD_DIR)/deps/xtrans
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto

# libX11 is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
# xorgproto comes FIRST, and the order matters rather than being cosmetic.
# xorgproto ships its own Xfuncproto.h and Xmd.h, newer than the xproto 7.0.31
# the xorg archive's newest tarball offers, and its extension headers require
# the newer macros.  With xproto first, libX11 picks up the old ones and dies
# with "error: expected declaration specifiers" 23 lines into XKBproto.h.
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_xorgproto)/include \
	-I$(PREFIX_libxcb)/include -I$(PREFIX_libxcb)/include/X11 -I$(PREFIX_libXau)/include -I$(PREFIX_libXdmcp)/include \
	-I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include \
	-I$(PREFIX_kbproto)/include -I$(PREFIX_inputproto)/include -I$(PREFIX_xtrans)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_libxcb)/lib -L$(PREFIX_libXau)/lib -L$(PREFIX_libXdmcp)/lib \
	-L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib -L$(PREFIX_xtrans)/lib

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/libX11.a) $(wildcard $(SRC_TREE)/src/.libs/libX11.a) $(wildcard $(SRC_TREE)/libX11.a) $(wildcard $(SRC_TREE)/.libs/libX11.a))

# .libs/ first: libtool writes a small ASCII "libtool library file" next to
# the real archive in the source directory, and picking that stages text.
FLX_XCB_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libX11-xcb.a $(SRC_TREE)/src/libX11-xcb.a))
BUILD_BIN   = $(PROJECT_LIB)

# Only our own .pc files, in dependency order.  Nothing from the host.


PROJECT_CFG_CMDS = \
	./configure \
		--prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME) \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--without-xml \
		--without-iconv \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_libxcb)/include -I$(PREFIX_libXau)/include -I$(PREFIX_libXdmcp)/include -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include " \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_libxcb)/lib -L$(PREFIX_libXau)/lib -L$(PREFIX_libXdmcp)/lib -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib "

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# A library stages itself: there is no single binary for install.mk to copy.
# One command per line -- a multi-line shell if here needs a backslash
# continuation that does not survive being generated.
install: all
	$(call flx-stage-pc,libX11,x11)

	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libX11/lib" "$(FREELINX_BUILD_DIR)/deps/libX11/include/."
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libX11/lib/libX11.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libX11/include/./"
	@test -d "$(SRC_TREE)/include" || printf '[FreeLinX/ports][error] %s: no headers at include/\n' "$(NAME)" >&2
	@# The pkg-config files are at the top of the tree and are called x11.pc
	@# and x11-xcb.pc, not libX11.pc: the module name every other port asks
	@# configure for is "x11", and PKG_CHECK_MODULES(x11) never looks at
	@# libX11.pc.  Looking only for libX11.pc staged nothing, and every
	@# downstream configure then failed with
	@#   Package 'x11' not found
	@# which names the consumer's requirement rather than the missing file.
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libX11/lib/pkgconfig"
	@for pc in x11 x11-xcb; do \
		test -f "$(SRC_TREE)/$$pc.pc" || { printf '[FreeLinX/ports][error] %s: %s.pc was not generated\n' "$(NAME)" "$$pc" >&2; exit 1; }; \
		$(INSTALL) -m 644 "$(SRC_TREE)/$$pc.pc" "$(FREELINX_BUILD_DIR)/deps/libX11/lib/pkgconfig/"; \
	done
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libX11/bin"; test ! -f "$(firstword $(wildcard $(SRC_TREE)/src/libX11-config $(SRC_TREE)/src/.libs/libX11-config))" || $(INSTALL) -m 755 "$(firstword $(wildcard $(SRC_TREE)/src/libX11-config $(SRC_TREE)/src/.libs/libX11-config))" "$(FREELINX_BUILD_DIR)/deps/libX11/bin/"
	# libX11-xcb.a is a second archive, not part of libX11.a.  Its header
	# Xlib-xcb.h is staged above, so a consumer can include it and then fail
	# to link XGetXCBConnection / XSetEventQueueOwner -- lemonbar includes it.
	@test -n "$(FLX_XCB_LIB)" || printf '[FreeLinX/ports][error] %s: libX11-xcb.a was not built\n' "$(NAME)" >&2
	@$(INSTALL) -m 644 "$(FLX_XCB_LIB)" "$(FREELINX_BUILD_DIR)/deps/libX11/lib/libX11-xcb.a"
	@printf '[FreeLinX/ports] staged libX11 into %s\n' "$(FREELINX_BUILD_DIR)/deps/libX11"
