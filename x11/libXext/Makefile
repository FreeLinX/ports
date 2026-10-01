# FreeLinX/ports - x11/libXext : libXext 1.3.7 (upstream project port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/lib/libXext-1.3.7.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# A library port.  It produces libXext.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := libXext
VERSION := 1.3.6
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= libX11 xproto xextproto
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

# libXext is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_xorgproto)/include -I$(PREFIX_libxcb)/include/X11 -I$(PREFIX_libX11)/include -I$(PREFIX_libXau)/include -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_libX11)/lib -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib

# xorgproto's headers are newer than the xproto 7.0.31 the xorg archive
# ships, and its extension headers need the newer macros, so its include
# directory goes first or Xext.h/Xrender.h fail to compile.
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_libxcb    = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libX11    = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_libXau = $(FREELINX_BUILD_DIR)/deps/libXau

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/libXext.a) $(wildcard $(SRC_TREE)/src/.libs/libXext.a) $(wildcard $(SRC_TREE)/libXext.a) $(wildcard $(SRC_TREE)/.libs/libXext.a))
BUILD_BIN   = $(PROJECT_LIB)

# Only our own .pc files, in dependency order.  Nothing from the host.


PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--without-xml \
		--without-iconv \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		PKG_CONFIG="/usr/bin/pkg-config" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_libX11)/include -I$(PREFIX_libXau)/include -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include " \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_libX11)/lib -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib "

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# A library stages itself: there is no single binary for install.mk to copy.
# One command per line -- a multi-line shell if here needs a backslash
# continuation that does not survive being generated.
install: all
	$(call flx-stage-pc,libXext,xext)

	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXext/lib" "$(FREELINX_BUILD_DIR)/deps/libXext/include/."
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libXext/lib/libXext.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libXext/include/./"
	@test -d "$(SRC_TREE)/include" || printf '[FreeLinX/ports][error] %s: no headers at include/\n' "$(NAME)" >&2
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXext/bin"; test ! -f "$(firstword $(wildcard $(SRC_TREE)/src/libXext-config $(SRC_TREE)/src/.libs/libXext-config))" || $(INSTALL) -m 755 "$(firstword $(wildcard $(SRC_TREE)/src/libXext-config $(SRC_TREE)/src/.libs/libXext-config))" "$(FREELINX_BUILD_DIR)/deps/libXext/bin/"
	@printf '[FreeLinX/ports] staged libXext into %s\n' "$(FREELINX_BUILD_DIR)/deps/libXext"
