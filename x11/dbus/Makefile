# FreeLinX/ports - x11/dbus : D-Bus message bus 1.16.2 (upstream project port).
#
# Real upstream source:
#   https://dbus.freedesktop.org/releases/dbus/dbus-1.16.2.tar.xz
#
# dbus is the freedesktop.org message bus.  It is not a GNU component and not a
# desktop one: anything that talks over D-Bus needs it, and a base system with
# no bus is a system where every desktop and service just quietly fails.
#
# Upstream is meson-only as of 1.16 -- there is no ./configure in the tarball --
# so this is the first FreeLinX port driven by meson.  It is C++, built against
# libc++ and libunwind from the toolchain rather than libstdc++ from GCC.
#
# Produces, all statically linked:
#   dbus-daemon            the bus itself
#   dbus-run-session       sets up a private bus for a login session
#   dbus-send              command-line client
#   dbus-monitor           bus traffic viewer
#   dbus-uuidgen           machine/session UUIDs
#   dbus-cleanup-sockets   stale socket reaper
#   libdbus-1.a            the client library, for later ports

NAME    := dbus
VERSION := 1.16.2
CATEGORY:= x11
LICENSE := AFL-2.1 OR GPL-2.0-or-later (dual licensed)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN := dbus-daemon
INSTALL_RELPATH := usr/bin/dbus-daemon

# dbus is C++, so the framework links libc++/libc++abi/libunwind.  Declared
# before the include: project-port.mk only adds them when it is set.
FLX_PROJECT_CXX = yes

include ../../mk/project-port.mk

# The meson option names are not guessable.  meson rejects an unknown one
# outright rather than ignoring it, and upstream spells them "installed_tests",
# "doxygen_docs", "xml_docs" and so on.  The full list is in the tarball's
# meson_options.txt.
#
# The types matter as much as the names.  An option declared `type: 'feature'`
# takes enabled/disabled/auto, and passing it "false" is an error, not a
# synonym: meson stops with 'Value "false" (of type "string") for option
# "modular_tests" is not one of the choices'.  Only options declared `boolean`
# take true/false.  So modular_tests, x11_autolaunch and ducktype_docs are
# disabled= below and checks/installed_tests/intrusive_tests are false=.
#
# The ones that matter for a static, no-systemd, no-selinux, no-X11 FreeLinX
# build:
#
#   --default-library=static      every archive ends up as libdbus-1.a
#   -D*_tests=false, -Dchecks=false
#                                 the suites build and *run* host executables,
#                                 which is meaningless in a cross build and is
#                                 not part of the deliverable
#   -Dselinux/-Dapparmor/-Dlibaudit=disabled
#                                 no policy in the image to load
#   -Dsystemd=disabled            no systemd here; runit services instead
#   -Dlaunchd=disabled            not macOS
#   -Dx11_autolaunch=false        no libX11 in the base image
#   -Ddoxygen_docs/-Dxml_docs     no docs in the image
#
# A cross file is mandatory, not decoration.  Without one meson believes it is
# configuring a native build and tries to *run* the sanity-check binary it just
# compiled, which is a musl static executable that cannot run on the build
# host; meson stops with "Could not invoke sanity test executable".  It is
# generated rather than checked in because it names absolute toolchain paths.
# pkgconfig is deliberately "false": there is no target pkg-config, and letting
# meson find the host's would let it locate host libraries and add
# /usr/include back, which is how a musl build ends up reading glibc headers.
#
# -Dc_args/-Dcpp_*-args carry $(FREELINX_RESOURCE_FLAGS): meson invokes the
# compiler directly rather than through a configure script, so nothing else
# puts -resource-dir on the line and the link picks up the host's GCC-built
# compiler-rt.  $(FREELINUX_CXX_FLAGS) is -nostdinc++ plus the sysroot's libc++
# include directory, which keeps the host's /usr/include/c++ out; without it
# clang++ falls back to libstdc++ and dbus comes out GCC-built.
# --rtlib=compiler-rt is not optional here, and its absence is invisible.
# Without it a *shared or PIE* link has clang reach for the GCC directory's
# crtbeginS.o/crtendS.o -- /usr/lib64/gcc/x86_64-pc-linux-gnu/16/ -- which
# GCC built.  Every dbus binary then carried "GCC: (GNU) 16.2.1" while every
# object in the link came from the toolchain and nothing GCC-built was named
# anywhere.  project-port.mk already knows this, which is why it passes
# -nostdlib and names musl's crt objects explicitly; meson invokes the
# compiler directly, so that knowledge has to be handed to it here.
# -static keeps the output consistent with every other FreeLinX binary.
FLX_LINK_ARGS = --target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) \
	$(FREELINX_RESOURCE_FLAGS) --rtlib=compiler-rt -unwindlib=none -static \
	-L$(FREELINX_SYSROOT)/lib

PROJECT_CFG_CMDS = \
	printf "[binaries]\nc = '%s'\ncpp = '%s'\nar = '%s'\nstrip = '%s'\npkgconfig = 'false'\n\n[host_machine]\nsystem = 'linux'\ncpu_family = 'x86_64'\ncpu = 'x86_64'\nendian = 'little'\n" \
		"$(FREELINX_CC)" "$(FREELINX_CXX)" "$(FREELINX_AR)" "$(FREELINX_TOOLCHAIN_BIN)/llvm-strip" \
		> "$(SRC_TREE)/freelinix.meson-cross" && \
	meson setup build \
		--cross-file "$(SRC_TREE)/freelinix.meson-cross" \
		--prefix=/usr \
		--libdir=/usr/lib \
		--sysconfdir=/etc \
		--datadir=/usr/share \
		--localstatedir=/var \
		--buildtype=release \
		--default-library=static \
		-Dchecks=false \
		-Dinstalled_tests=false \
		-Dintrusive_tests=false \
		-Dmodular_tests=disabled \
		-Dvalgrind=disabled \
		-Dselinux=disabled \
		-Dapparmor=disabled \
		-Dlibaudit=disabled \
		-Dsystemd=disabled \
		-Dlaunchd=disabled \
		-Dx11_autolaunch=disabled \
		-Ddoxygen_docs=disabled \
		-Dxml_docs=disabled \
		-Dducktype_docs=disabled \
		-Dtools=true \
		-Dmessage_bus=true \
		-Dc_args="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) $(FREELINX_RESOURCE_FLAGS)" \
		-Dcpp_args="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) $(FREELINX_RESOURCE_FLAGS) -stdlib=libc++ $(FREELINUX_CXX_FLAGS)" \
		-Dc_link_args="$(FLX_LINK_ARGS)" \
		-Dcpp_link_args="$(FLX_LINK_ARGS)"

# The tests are off, so there is nothing to skip: one ninja invocation produces
# every binary and library this port delivers.
PROJECT_BIN = $(SRC_TREE)/build/bus/dbus-daemon
BUILD_BIN   = $(PROJECT_BIN)

BUILD_CMDS = \
	ninja -C build

# Six binaries plus the client library.  The framework stages only
# INSTALL_RELPATH, so the rest are staged here.
# meson does not leave the binaries in one directory.  The daemon and the
# session launcher come from bus/, the client tools from tools/, and the
# client library from dbus/.  Guessing a single $(SRC_TREE)/build path finds
# nothing at all, and an install loop that treats "not found" as "skip" then
# reports success having staged six nothing.
DBUS_BUILD = $(SRC_TREE)/build
DBUS_BINS = bus/dbus-daemon \
	tools/dbus-run-session tools/dbus-send \
	tools/dbus-monitor tools/dbus-uuidgen \
	tools/dbus-cleanup-sockets
DBUS_LIBS = dbus/libdbus-1.a

install: $(STAGE_FILE)
	@for b in $(DBUS_BINS); do \
		if [ ! -x "$(DBUS_BUILD)/$$b" ]; then \
			printf '[FreeLinX/ports][error] dbus: %s was not built\n' "$$b" >&2; \
			exit 1; \
		fi; \
		n=$$(basename $$b); \
		printf '[FreeLinX/ports] staging dbus %s -> usr/bin/%s\n' "$$b" "$$n"; \
		$(MKDIR) -p "$(STAGE_ROOT)/usr/bin"; \
		$(INSTALL) -m 755 "$(DBUS_BUILD)/$$b" "$(STAGE_ROOT)/usr/bin/$$n" || exit 1; \
	done
	@for l in $(DBUS_LIBS); do \
		if [ -f "$(DBUS_BUILD)/$$l" ]; then \
			n=$$(basename $$l); \
			$(MKDIR) -p "$(STAGE_ROOT)/usr/lib"; \
			$(INSTALL) -m 644 "$(DBUS_BUILD)/$$l" "$(STAGE_ROOT)/usr/lib/$$n"; \
		fi; \
	done

install-rootfs: check-install-rootfs $(STAGE_FILE)
	@printf '[FreeLinX/ports] installing dbus into rootfs\n'
	$(MKDIR) -p "$(dir $(ROOTFS_FILE))"
	$(INSTALL) -m 755 $(STAGE_FILE) $(ROOTFS_FILE)
	@for b in $(DBUS_BINS); do \
		n=$$(basename $$b); \
		if [ ! -f "$(STAGE_ROOT)/usr/bin/$$n" ]; then \
			printf '[FreeLinX/ports][error] dbus: %s was not staged\n' "$$n" >&2; \
			exit 1; \
		fi; \
		$(MKDIR) -p "$(ROOTFS_DIR)/usr/bin"; \
		$(INSTALL) -m 755 "$(STAGE_ROOT)/usr/bin/$$n" "$(ROOTFS_DIR)/usr/bin/$$n"; \
	done
	@for l in $(DBUS_LIBS); do \
		n=$$(basename $$l); \
		if [ -f "$(STAGE_ROOT)/usr/lib/$$n" ]; then \
			$(MKDIR) -p "$(ROOTFS_DIR)/usr/lib"; \
			$(INSTALL) -m 644 "$(STAGE_ROOT)/usr/lib/$$n" "$(ROOTFS_DIR)/usr/lib/$$n"; \
		fi; \
	done


