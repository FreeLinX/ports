# FreeLinX/ports - x11/libXkbfile : XKB file manipulation library 1.1.3.
#
# Real upstream source:
#   https://xorg.freedesktop.org/archive/individual/lib/libxkbfile-1.1.3.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Produces libxkbfile.a (upstream spelling, see PROJECT_LIB).  xkbcomp links it, and so does xorg-server, so it
# comes before both.
#
# MIT licensed.

NAME    := libXkbfile
VERSION := 1.1.3
CATEGORY:= x11
LICENSE := MIT (X11)
DEPENDENCIES:= libX11 libXau libXdmcp
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

PREFIX_libX11   = $(FREELINX_BUILD_DIR)/deps/libX11
PREFIX_libxcb   = $(FREELINX_BUILD_DIR)/deps/libxcb
PREFIX_libXau   = $(FREELINX_BUILD_DIR)/deps/libXau
PREFIX_libXdmcp = $(FREELINX_BUILD_DIR)/deps/libXdmcp
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_xtrans   = $(FREELINX_BUILD_DIR)/deps/xtrans

FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

# xorgproto first: its Xfuncproto.h and Xmd.h are newer than the xproto 7.0.31
# the xorg archive publishes, and XKFile.h needs the newer macros.
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_xorgproto)/include \
	-I$(PREFIX_libX11)/include -I$(PREFIX_libXau)/include \
	-I$(PREFIX_libXdmcp)/include -I$(PREFIX_xtrans)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_libX11)/lib -L$(PREFIX_libXau)/lib \
	-L$(PREFIX_libXdmcp)/lib

include ../../mk/project-port.mk

# The tarball's root directory is libxkbfile-1.1.3, lower-case x, while the port
# is named libXkbfile.  SRC_TREE defaults to work/$(NAME)/$(DISTINFO_NAME), so
# without this the recipes cd into a directory that was never created and the
# build stops at "cd: .../libXkbfile-1.1.3: No such file or directory".
SRC_TREE = $(SRC_DIR)/libxkbfile-$(VERSION)

# Upstream names this archive libxkbfile.a, all lower case -- src/Makefile.am
# says lib_LTLIBRARIES = libxkbfile.la and configure.ac says
# AC_INIT([libxkbfile], ...).  So consumers link -lxkbfile, and the staged file
# keeps that spelling; renaming it to libXkbfile.a here would only work until
# the first -lXkbfile.
PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/src/.libs/libxkbfile.a $(SRC_TREE)/src/libxkbfile.a))
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=$(FREELINX_BUILD_DIR)/deps/$(NAME) \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC $(FLX_PROJECT_CPPFLAGS)" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt $(FLX_PROJECT_LDFLAGS)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXkbfile/lib/pkgconfig" \
		"$(FREELINX_BUILD_DIR)/deps/libXkbfile/include"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libXkbfile/lib/libxkbfile.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libXkbfile/include/"
	@test ! -f "$(SRC_TREE)/xkbfile.pc" || $(INSTALL) -m 644 "$(SRC_TREE)/xkbfile.pc" "$(FREELINX_BUILD_DIR)/deps/libXkbfile/lib/pkgconfig/xkbfile.pc"
	@printf '[FreeLinX/ports] staged libxkbfile.a into %s\n' "$(FREELINX_BUILD_DIR)/deps/libXkbfile"
