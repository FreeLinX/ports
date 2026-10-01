# FreeLinX/ports - x11/libXft : Xft 2.3.9 (upstream project port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/lib/libXft-2.3.9.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Produces libXft.a.  This is the last library before lemonbar: lemonbar needs
# libX11, libxcb and libXft, and libXft needs libX11, libXrender, fontconfig and
# freetype -- all built.
#
# libXft is MIT-licensed.

NAME    := libXft
VERSION := 2.3.9
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libxcb libXrender fontconfig freetype2
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

PREFIX_libX11    = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_libxcb    = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libXau    = $(FREELINX_BUILD_DIR)/deps/libXau
PREFIX_libXdmcp  = $(FREELINX_BUILD_DIR)/deps/libXdmcp
PREFIX_libXrender = $(FREELINX_BUILD_DIR)/deps/libXrender
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_xtrans    = $(FREELINX_BUILD_DIR)/deps/xtrans
PREFIX_fontconfig = $(FREELINX_BUILD_DIR)/deps/fontconfig
# freetype2 stages into the overlay, not build/deps.
PREFIX_freetype2 = $(FREELINX_STAGING_ROOT)/usr

# libXft is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

# xorgproto first: its Xfuncproto.h and Xmd.h are newer than the xproto 7.0.31
# the xorg archive publishes, and everything here needs the newer macros.
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_xorgproto)/include \
	-I$(PREFIX_libX11)/include -I$(PREFIX_libxcb)/include/X11 \
	-I$(PREFIX_libXau)/include -I$(PREFIX_libXdmcp)/include \
	-I$(PREFIX_libXrender)/include -I$(PREFIX_xtrans)/include \
	-I$(PREFIX_fontconfig)/include -I$(PREFIX_freetype2)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_libX11)/lib -L$(PREFIX_libxcb)/lib \
	-L$(PREFIX_libXau)/lib -L$(PREFIX_libXdmcp)/lib -L$(PREFIX_libXrender)/lib \
	-L$(PREFIX_fontconfig)/lib -L$(PREFIX_freetype2)/lib

include ../../mk/project-port.mk

# The .la is not a candidate: libtool writes a 1 KB ASCII text file next to the
# real archive, and picking that stages a text file as libXft.a.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libXft.a $(SRC_TREE)/src/libXft.a))
BUILD_BIN   = $(PROJECT_LIB)



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
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC $(FLX_PROJECT_CPPFLAGS)" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt $(FLX_PROJECT_LDFLAGS)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" LIBS="-lXrender -lfontconfig -lfreetype"

install: all
	$(call flx-stage-pc,libXft,xft)

	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXft/lib" "$(FREELINX_BUILD_DIR)/deps/libXft/include/X11"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libXft/lib/libXft.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libXft/include/"
	@if [ -f "$(SRC_TREE)/src/libXft.pc" ]; then \
		mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXft/lib/pkgconfig"; \
		$(INSTALL) -m 644 "$(SRC_TREE)/src/libXft.pc" "$(FREELINX_BUILD_DIR)/deps/libXft/lib/pkgconfig/"; \
	fi
	@printf '[FreeLinX/ports] staged libXft into %s\n' "$(FREELINX_BUILD_DIR)/deps/libXft"
