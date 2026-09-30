# FreeLinX/ports - devel/expat : XML parser 2.6.4 (upstream project port).
#
# Real upstream source:
#   https://github.com/libexpat/libexpat/releases/download/R_2_6_4/expat-2.6.4.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Produces libexpat.a.  fontconfig 2.15 made expat mandatory -- its configure
# says "*** expat is required. or try to use --enable-libxml2" and there is no
# --without-expat any more -- so this is on the critical path to lemonbar.
# libXft needs fontconfig, lemonbar needs libXft.
#
# expat is MIT, not GPL, and is the XML parser used across the tree.

NAME    := expat
VERSION := 2.6.4
CATEGORY:= devel
LICENSE := MIT
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

# expat is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/build/libexpat.a $(SRC_TREE)/libexpat.a))
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	cmake -S "$(SRC_TREE)" -B "$(SRC_TREE)/build" \
		-DCMAKE_SYSTEM_NAME=Linux \
		-DCMAKE_SYSTEM_PROCESSOR=x86_64 \
		-DCMAKE_C_COMPILER="$(FREELINX_CC)" \
		-DCMAKE_C_COMPILER_TARGET=$(FREELINX_TRIPLE) \
		-DCMAKE_C_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC" \
		-DCMAKE_SYSROOT="$(FREELINX_SYSROOT)" \
		-DCMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES="$(FREELINX_SYSROOT)/include" \
		-DCMAKE_AR="$(FREELINX_AR)" \
		-DCMAKE_RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
		-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
		-DEXPAT_BUILD_TOOLS=OFF \
		-DEXPAT_BUILD_EXAMPLES=OFF \
		-DEXPAT_BUILD_TESTS=OFF \
		-DEXPAT_BUILD_DOCS=OFF \
		-DEXPAT_SHARED_LIBS=OFF \
		-DEXPAT_BUILD_STATIC_LIBS=ON \
		-DEXPAT_INSTALL=ON

BUILD_CMDS = \
	cmake --build "$(SRC_TREE)/build" -j"$(FREELINX_JOBS)"

install: all
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/expat/lib" "$(FREELINX_BUILD_DIR)/deps/expat/include"
	@test -n "$(PROJECT_LIB)" || printf '[FreeLinX/ports][error] %s: libexpat.a was not built\n' "$(NAME)" >&2
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/expat/lib/libexpat.a"
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/expat/include"
	# The public headers are under lib/, not at the top level: the top level has
	# expat_config.h, which is a build artefact, not something a consumer includes.
	@cp $(SRC_TREE)/lib/expat.h $(SRC_TREE)/lib/expat_external.h "$(FREELINX_BUILD_DIR)/deps/expat/include/"
	@# expat.pc, because fontconfig finds expat with pkg-config and not with a
	@# -I/-L pair: its configure stops at "*** expat is required. or try to use
	@# --enable-libxml2" when the .pc is not on PKG_CONFIG_PATH.
	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/expat/lib/pkgconfig"
	@$(INSTALL) -m 644 "$(SRC_TREE)/build/NoConfig/expat.pc" "$(FREELINX_BUILD_DIR)/deps/expat/lib/pkgconfig/"
	# xmlparse.h, the expat 1.x header name.  expat 2.x dropped it: 2.6.4 ships
	# lib/expat.h and lib/xmlparse.c but no xmlparse.h.  fontconfig 2.15's configure
	# still probes for expat with #include <xmlparse.h>, so without this it reports
	# "expat is required. or try to use --enable-libxml2" on a system that has
	# expat installed and visible to pkg-config.  The shim is all the original
	# header ever was.
	@printf '#include <expat.h>\n' > "$(FREELINX_BUILD_DIR)/deps/expat/include/xmlparse.h"
	@printf '[FreeLinX/ports] staged expat into %s\n' "$(FREELINX_BUILD_DIR)/deps/expat"
