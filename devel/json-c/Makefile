# FreeLinX/ports - devel/json-c : JSON parser 0.19 (upstream project port).
#
# Real upstream source, from json-c's own GitHub release:
#   https://github.com/json-c/json-c/releases/download/json-c-0.19-20260627/json-c-0.19.tar.gz
#
# The tags json-c-0.17 and json-c-0.18 on GitHub both 404; the release assets
# are what actually exist, so distinfo points at the release download rather
# than an archive/refs/tags URL.
#
# Built statically with FreeLinX clang + LLD + musl.
#
# Produces libjson-c.a.  fontconfig 2.15 moved its cache format to json-c and
# its link ends at "unable to find library -ljson-c", so this is what stands
# between fontconfig and libXft and lemonbar.
#
# json-c is MIT.

NAME    := json-c
VERSION := 0.19
CATEGORY:= devel
LICENSE := MIT
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

# json-c is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

PROJECT_LIB = $(firstword $(wildcard $(SRC_TREE)/build/libjson-c.a $(SRC_TREE)/build/lib/libjson-c.a))
BUILD_BIN   = $(PROJECT_LIB)

# -rtlib=compiler-rt and -static live in the LINKER flags, not CMAKE_C_FLAGS.
# In CFLAGS clang accepts them at compile time and does not use them, so it
# warns; json-c builds its own test tools with -Werror and the build stops at
# "argument unused during compilation: '--rtlib=compiler-rt'".  configure-based
# ports get away with it because configure passes CFLAGS and LDFLAGS separately.
#
# CMAKE_C_COMPILER_TARGET is not optional.  Without it CMake resolves the
# target triple from the compiler binary, which here is a symlink to the
# host's, and configure-equivalent sanity checking stops with
# "error: unknown target triple 'unknown'".
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
		-DCMAKE_EXE_LINKER_FLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -unwindlib=none" \
		-DCMAKE_SHARED_LINKER_FLAGS="-static -fuse-ld=lld --rtlib=compiler-rt -unwindlib=none" \
		-DBUILD_SHARED_LIBS=OFF \
		-DBUILD_STATIC_LIBS=ON \
		-DBUILD_TESTING=OFF \
		-DJSONC_BUILD_DOCS=OFF

BUILD_CMDS = \
	cmake --build "$(SRC_TREE)/build" -j"$(FREELINX_JOBS)"

install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/json-c/lib/pkgconfig" "$(FREELINX_BUILD_DIR)/deps/json-c/include/json"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/json-c/lib/libjson-c.a"
	@cp $(SRC_TREE)/json_object.h $(SRC_TREE)/json_tokener.h \
		$(SRC_TREE)/json_util.h $(SRC_TREE)/json_object_iterator.h \
		$(SRC_TREE)/json_pointer.h $(SRC_TREE)/arraylist.h \
		$(SRC_TREE)/linkhash.h $(SRC_TREE)/printbuf.h \
		$(SRC_TREE)/debug.h "$(FREELINX_BUILD_DIR)/deps/json-c/include/json/"
	@if [ -f "$(SRC_TREE)/json-c.pc.in" ]; then \
		sed -e 's|@prefix@|$(FREELINX_BUILD_DIR)/deps/json-c|g' \
		    -e 's|@includedir@|$${prefix}/include|g' \
		    "$(SRC_TREE)/json-c.pc.in" > "$(FREELINX_BUILD_DIR)/deps/json-c/lib/pkgconfig/json-c.pc"; \
	fi
	@printf '[FreeLinX/ports] staged json-c into %s\n' "$(FREELINX_BUILD_DIR)/deps/json-c"
