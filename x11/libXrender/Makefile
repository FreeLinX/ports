# FreeLinX/ports - x11/libXrender : libXrender 0.9.12 (upstream project port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/lib/libXrender-0.9.12.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# A library port.  It produces libXrender.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := libXrender
VERSION := 0.9.12
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libX11 xproto xextproto renderproto
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
PREFIX_libX11 = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_xproto = $(FREELINX_BUILD_DIR)/deps/xproto
PREFIX_xextproto = $(FREELINX_BUILD_DIR)/deps/xextproto

# libXrender is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_xorgproto)/include -I$(PREFIX_libxcb)/include/X11 -I$(PREFIX_libX11)/include -I$(PREFIX_renderproto)/include -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_libX11)/lib -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib

# xorgproto's headers are newer than the xproto 7.0.31 the xorg archive
# ships, and its extension headers need the newer macros, so its include
# directory goes first or Xext.h/Xrender.h fail to compile.
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_libxcb    = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libX11    = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_renderproto = $(FREELINX_BUILD_DIR)/deps/renderproto

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/libXrender.a) $(wildcard $(SRC_TREE)/src/.libs/libXrender.a) $(wildcard $(SRC_TREE)/libXrender.a) $(wildcard $(SRC_TREE)/.libs/libXrender.a))
BUILD_BIN   = $(PROJECT_LIB)

# Only our own .pc files, in dependency order.  Nothing from the host.
PKG_PATH = $(FREELINX_BUILD_DIR)/deps/xorgproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/libX11/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xextproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/renderproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xproto/lib/pkgconfig

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--without-xml \
		--without-iconv \
		PKG_CONFIG_PATH="$(PKG_PATH)" \
		PKG_CONFIG="/usr/bin/pkg-config" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_libX11)/include -I$(PREFIX_renderproto)/include -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include " \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_libX11)/lib -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib "

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# A library stages itself: there is no single binary for install.mk to copy.
# One command per line -- a multi-line shell if here needs a backslash
# continuation that does not survive being generated.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXrender/lib" "$(FREELINX_BUILD_DIR)/deps/libXrender/include/."
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libXrender/lib/libXrender.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libXrender/include/./"
	@test -d "$(SRC_TREE)/include" || printf '[FreeLinX/ports][error] %s: no headers at include/\n' "$(NAME)" >&2
	@test ! -f "$(firstword $(wildcard $(SRC_TREE)/src/libXrender.pc $(SRC_TREE)/src/.libs/libXrender.pc))" || $(INSTALL) -m 644 "$(firstword $(wildcard $(SRC_TREE)/src/libXrender.pc $(SRC_TREE)/src/.libs/libXrender.pc))" "$(FREELINX_BUILD_DIR)/deps/libXrender/lib/"
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXrender/bin"; test ! -f "$(firstword $(wildcard $(SRC_TREE)/src/libXrender-config $(SRC_TREE)/src/.libs/libXrender-config))" || $(INSTALL) -m 755 "$(firstword $(wildcard $(SRC_TREE)/src/libXrender-config $(SRC_TREE)/src/.libs/libXrender-config))" "$(FREELINX_BUILD_DIR)/deps/libXrender/bin/"
	@printf '[FreeLinX/ports] staged libXrender into %s\n' "$(FREELINX_BUILD_DIR)/deps/libXrender"
