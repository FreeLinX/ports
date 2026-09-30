# FreeLinX/ports - x11/libXau : libXau 1.0.12 (upstream project port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/lib/libXau-1.0.12.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# A library port.  It produces libXau.a into the build/deps overlay,
# and the next port in the dependency line names that prefix.  It stages
# nothing into the rootfs: a .a is build-time only, and shipping one into
# /usr/lib would be dead weight in a static image.

NAME    := libXau
VERSION := 1.0.12
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= xproto xextproto
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
PREFIX_xproto = $(FREELINX_BUILD_DIR)/deps/xproto
PREFIX_xextproto = $(FREELINX_BUILD_DIR)/deps/xextproto

# libXau is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS = -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include
FLX_PROJECT_LDFLAGS  = -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/./libXau.a) $(wildcard $(SRC_TREE)/./.libs/libXau.a) $(wildcard $(SRC_TREE)/libXau.a) $(wildcard $(SRC_TREE)/.libs/libXau.a))
BUILD_BIN   = $(PROJECT_LIB)

# Only our own .pc files, in dependency order.  Nothing from the host.
PKG_PATH = $(FREELINX_BUILD_DIR)/deps/libXau/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xextproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xproto/lib/pkgconfig:$(FREELINX_BUILD_DIR)/deps/xextproto/lib/pkgconfig

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
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include " \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PREFIX_xproto)/lib -L$(PREFIX_xextproto)/lib "

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

# A library stages itself: there is no single binary for install.mk to copy.
# One command per line -- a multi-line shell if here needs a backslash
# continuation that does not survive being generated.
install: all
	@test -n "$(PROJECT_LIB)" || printf '[FreeLinX/ports][error] %s: libXau.a was not built\n' "$(NAME)" >&2
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXau/lib" "$(FREELINX_BUILD_DIR)/deps/libXau/include/."
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/libXau/lib/libXau.a"
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/libXau/include/./"
	@test -d "$(SRC_TREE)/include" || printf '[FreeLinX/ports][error] %s: no headers at include/\n' "$(NAME)" >&2
	@test ! -f "$(firstword $(wildcard $(SRC_TREE)/./libXau.pc $(SRC_TREE)/./.libs/libXau.pc))" || $(INSTALL) -m 644 "$(firstword $(wildcard $(SRC_TREE)/./libXau.pc $(SRC_TREE)/./.libs/libXau.pc))" "$(FREELINX_BUILD_DIR)/deps/libXau/lib/"
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/libXau/bin"; test ! -f "$(firstword $(wildcard $(SRC_TREE)/./libXau-config $(SRC_TREE)/./.libs/libXau-config))" || $(INSTALL) -m 755 "$(firstword $(wildcard $(SRC_TREE)/./libXau-config $(SRC_TREE)/./.libs/libXau-config))" "$(FREELINX_BUILD_DIR)/deps/libXau/bin/"
	@printf '[FreeLinX/ports] staged libXau into %s\n' "$(FREELINX_BUILD_DIR)/deps/libXau"
