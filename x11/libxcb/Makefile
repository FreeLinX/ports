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

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/libxcb.a $(SRC_TREE)/src/.libs/libxcb.a))
BUILD_BIN   = $(PROJECT_LIB)

PKG_PATH = $(FREELINX_BUILD_DIR)/deps/libXdmcp/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/libXau/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xextproto/lib/pkgconfig

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		--disable-doxygen-docs \
		--disable-composite-output \
		PKG_CONFIG_PATH="$(PKG_PATH)" \
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
	@test -n "$(PROJECT_LIB)" || printf '[FreeLinX/ports][error] %s: libxcb.a was not built\n' "$(NAME)" >&2
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libxcb/lib/pkgconfig" "$(FREELINX_BUILD_DIR)/deps/libxcb/include/X11/xcb"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libxcb/lib/libxcb.a"
	@cp $(SRC_TREE)/src/*.h "$(FREELINX_BUILD_DIR)/deps/libxcb/include/X11/xcb/"
	@$(INSTALL) -m 644 "$(SRC_TREE)/xcb.pc" "$(FREELINX_BUILD_DIR)/deps/libxcb/lib/pkgconfig/"
	@printf '[FreeLinX/ports] staged libxcb into %s\n' "$(FREELINX_BUILD_DIR)/deps/libxcb"
