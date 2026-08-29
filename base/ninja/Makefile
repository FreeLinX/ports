# FreeLinX/ports - base/ninja : Ninja v1.13.2 (upstream project port).
#
# Ninja is a self-contained C++11 build system with no external dependencies
# beyond the C++ standard library.  It is built from the real upstream source
# with CMake (host cmake drives the cross-compile) against the FreeLinX
# clang + musl libc++ + LLD toolchain, and links statically.
#
# The resulting binary is a genuine, statically-linked Ninja with zero GNU
# runtime dependency (libc++/libc++abi/libunwind/compiler-rt, all from the
# FreeLinX LLVM toolchain).  It is installed at /bin/ninja in the FreeLinX
# rootfs and can drive real build manifests at runtime.
#
# Known limitation: the upstream `browse` (browse.py HTML UI) component is
# header-only and always built; no Python is required at build OR runtime for
# the `ninja` executable itself (Python is only a build-time convenience of
# configure.py, which we bypass by using CMake).

NAME   := ninja
VERSION:= 1.13.2
CATEGORY:= base
LICENSE:= Apache-2.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl libc++ (via FreeLinX toolchain); host cmake + ninja
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ninja
INSTALL_RELPATH:= bin/ninja

# Source + build layout (project-port.mk derives SRC_TREE from this).
# `=` (recursive): FREELINX_BUILD_DIR is only bound during project-port.mk's
# include, so these must resolve lazily, not at Makefile-parse time.
PROJECT_BIN = $(FREELINX_BUILD_DIR)/obj/$(NAME)/ninja
# Build out-of-source into build/obj/ninja so SRC_TREE stays pristine.
PROJECT_BUILD_DIR = $(FREELINX_BUILD_DIR)/obj/$(NAME)

# Ninja's CMakeLists is C++-only; drive it with the FreeLinX C++ toolchain.
PROJECT_CFG_CMDS = cmake -S "$(SRC_TREE)" -B "$(PROJECT_BUILD_DIR)" \
	-G Ninja \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER="$(FREELINX_CC)" \
	-DCMAKE_CXX_COMPILER="$(FREELINX_CXX)" \
	-DCMAKE_CXX_FLAGS="$(CXXFLAGS)" \
	-DCMAKE_EXE_LINKER_FLAGS="$(LDFLAGS)" \
	-DBUILD_TESTING=OFF

BUILD_CMDS = cmake --build "$(PROJECT_BUILD_DIR)"

include ../../mk/project-port.mk

.PHONY: do-prepare
do-prepare:
	@: # extracted by project-port.mk do-extract
