# FreeLinX/ports - x11/libxcb : X11 protocol library 1.17.0 (upstream port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/xcb/libxcb-1.17.0.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Produces libxcb.a.  Three things about this package are not the same shape as
# its neighbours in the X11 line, which is why it is a hand-written port rather
# than one of the generated ones:
#
#   * xorg serves it from /pub/individual/xcb/, not /pub/individual/lib/.
#   * It has no include/ directory.  The headers are src/*.h, and upstream's own
#     install rule puts them in $(includedir)/X11/xcb/, so they have to be
#     staged there too -- otherwise every downstream #include <X11/xcb/xcb.h>
#     fails.
#   * Its pkg-config file is xcb.pc, not libxcb.pc.

NAME    := libxcb
VERSION := 1.17.0
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libXdmcp libXau xproto xextproto
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

PREFIX_libXdmcp  = $(FREELINX_BUILD_DIR)/deps/libXdmcp
PREFIX_libXau    = $(FREELINX_BUILD_DIR)/deps/libXau
PREFIX_xproto    = $(FREELINX_BUILD_DIR)/deps/xproto
PREFIX_xextproto = $(FREELINX_BUILD_DIR)/deps/xextproto

FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_libXdmcp)/include -I$(PREFIX_libXau)/include -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_libXdmcp)/lib -L$(PREFIX_libXau)/lib -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libxcb.a $(SRC_TREE)/src/libxcb.a))

# The extensions land in their own archives, not in libxcb.a: libxcb's build
# prints "Randr...............: yes" and "GEN randr.c" and then produces
# libxcb-randr.a beside libxcb.a.  Staging only $(PROJECT_LIB) leaves lemonbar
# with the randr.h header present and no implementation behind it, which is a
# link error a long way from the header that invited it.
FLX_EXT_LIBS = $(wildcard $(SRC_TREE)/src/.libs/libxcb-*.a)
BUILD_BIN   = $(PROJECT_LIB)



PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--disable-doxygen-docs \
		--enable-randr \
		--enable-xinerama \
		--enable-present \
		--enable-xinput \
		--enable-xkb \
		--enable-util \
		--disable-composite-output \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		PKG_CONFIG="/usr/bin/pkg-config" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# xcb.pc, not libxcb.pc: libX11's configure asks pkg-config for
# "xproto >= 7.0.25 xextproto xtrans xcb >= 1.11.1", so a port that stages its
# archive under one name and its .pc under another breaks the next link in the
# chain with a message that names neither.
install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libxcb/lib/pkgconfig" "$(FREELINX_BUILD_DIR)/deps/libxcb/include/X11/xcb"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libxcb/lib/libxcb.a"
	@cp $(SRC_TREE)/src/*.h "$(FREELINX_BUILD_DIR)/deps/libxcb/include/X11/xcb/"
	@$(INSTALL) -m 644 "$(SRC_TREE)/xcb.pc" "$(FREELINX_BUILD_DIR)/deps/libxcb/lib/pkgconfig/"
	# The extension archives, staged beside libxcb.a.  These are what back the
	# randr.h and xinerama.h copied just above; without them a consumer that
	# includes either header links against nothing.
	@for e in $(FLX_EXT_LIBS); do \
		$(INSTALL) -m 644 "$$e" "$(FREELINX_BUILD_DIR)/deps/libxcb/lib/"; \
	done
	@printf '[FreeLinX/ports] staged libxcb into %s\n' "$(FREELINX_BUILD_DIR)/deps/libxcb"
