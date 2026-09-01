# FreeLinX/ports - base/fastfetch : fastfetch 2.67.1 (upstream project port).
#
# fastfetch: feature-rich, neofetch-like system information tool, written in C.
# License: MIT (non-GNU).
#
# Built from the real upstream source with the FreeLinX clang + LLD + musl
# toolchain via its own CMake build system, fully static.
#
# Dependency policy (FreeLinX minimal static build):
#   - Zero external libraries. fastfetch bundles yyjson (ENABLE_SYSTEM_YYJSON=OFF)
#     and reads hardware/OS state from the running kernel (/proc, /sysfs,
#     /etc/os-release).  Everything it reports is real kernel data; nothing is
#     fabricated.
#   - All optional graphical/session/DBus/driver libraries are disabled
#     (X11/Wayland/DRM/Vulkan/GL/EGL/OpenCL/Pulse/DBus/DConf/GIO/SQLite/RPM/
#     ImageMagick/chafa/Lua/QuickJS/libzfs/ELF/zlib/ddcutil/...).  Each is only a
#     detection *enhancement*; fastfetch still reports the core OS/kernel/CPU/
#     memory/uptime/disk/package info from /proc and /sysfs without them.
#   - Both `fastfetch` and its companion `flashfetch` are delivered.
#
# Single source patch (patches/patch-cmake-ldl-copy-dt-needed): fastfetch's
# CMake adds `-Wl,--copy-dt-needed-entries` for non-dlopen link types, a GNU ld
# only option that LLD does not implement (and which is the default behaviour
# anyway).  It is omitted so the static musl + LLD link succeeds.

NAME   := fastfetch
VERSION:= 2.67.1
CATEGORY:= base
LICENSE:= MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot); host cmake + ninja
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= fastfetch
INSTALL_RELPATH:= bin/fastfetch

# fastfetch is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_LIBS :=

# Source + build layout (project-port.mk derives SRC_TREE from this).
PROJECT_BIN = $(FREELINX_BUILD_DIR)/obj/$(NAME)/fastfetch
BUILD_BIN   = $(PROJECT_BIN)
PROJECT_BUILD_DIR = $(FREELINX_BUILD_DIR)/obj/$(NAME)

# Static-link flags for CMake-driven project builds (git/ninja convention).
FF_LDFLAGS = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) --rtlib=compiler-rt -static -fuse-ld=lld
FF_CFLAGS  = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2

# Apply the single FreeLinX patch (idempotent), then configure out-of-source.
define FF_PATCH_AND_CONFIGURE
	if ! grep -q "FreeLinX: -Wl,--copy-dt-needed-entries" "CMakeLists.txt"; then \
		patch -d . -p1 < "$(FREELINX_PORT_PATCH)"; \
	fi; \
	cmake -S "$(SRC_TREE)" -B "$(PROJECT_BUILD_DIR)" -G Ninja \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_C_COMPILER="$(FREELINX_CC)" \
		-DCMAKE_C_FLAGS="$(FF_CFLAGS)" \
		-DCMAKE_EXE_LINKER_FLAGS="$(FF_LDFLAGS)" \
		-DBUILD_TESTS=OFF -DBUILD_FLASHFETCH=ON -DENABLE_LTO=OFF -DIS_MUSL=ON \
		-DBINARY_LINK_TYPE=static \
		-DENABLE_VULKAN=OFF -DENABLE_WAYLAND=OFF -DENABLE_XCB_RANDR=OFF -DENABLE_XRANDR=OFF \
		-DENABLE_DRM=OFF -DENABLE_VADRM=OFF -DENABLE_VAX11=OFF -DENABLE_VDPAU=OFF \
		-DENABLE_GIO=OFF -DENABLE_DCONF=OFF -DENABLE_EET=OFF -DENABLE_DBUS=OFF \
		-DENABLE_SQLITE3=OFF -DENABLE_RPM=OFF \
		-DENABLE_IMAGEMAGICK7=OFF -DENABLE_IMAGEMAGICK6=OFF -DENABLE_CHAFA=OFF \
		-DENABLE_EGL=OFF -DENABLE_GLX=OFF -DENABLE_OPENCL=OFF -DENABLE_FREETYPE=OFF \
		-DENABLE_PULSE=OFF -DENABLE_DDCUTIL=OFF -DENABLE_ELF=OFF \
		-DENABLE_ZLIB=OFF -DENABLE_SYSTEM_YYJSON=OFF \
		-DENABLE_LIBZFS=OFF -DENABLE_LUA=OFF -DENABLE_QUICKJS=OFF \
		-DENABLE_EMBEDDED_PCIIDS=OFF -DENABLE_EMBEDDED_AMDGPUIDS=OFF
endef

FREELINX_PORT_PATCH = $(CURDIR)/patches/patch-cmake-ldl-copy-dt-needed
# Recursive (=): FREELINX_CC / SRC_TREE / PROJECT_BUILD_DIR are only bound after
# the project-port.mk include below, so defer expansion until recipe-run time.
PROJECT_CFG_CMDS = $(FF_PATCH_AND_CONFIGURE)

BUILD_CMDS = cmake --build "$(PROJECT_BUILD_DIR)" --target fastfetch flashfetch

include ../../mk/project-port.mk
