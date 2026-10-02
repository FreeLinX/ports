# FreeLinX/ports - sysutils/usbutils : usbutils 018 (USB utilities).
#
# Provides lsusb for USB bus device identification.  Meson build system.
#
# Not buildable yet, and the port says so rather than failing four steps later.
# usbutils 018 links lsusb against two libraries, and this tree has a port of
# only one of them:
#
#   meson.build:107  libudev = dependency('libudev', version: '>= 196')
#   meson.build:112  executable('lsusb', lsusb_sources, dependencies: [libusb, libudev])
#
# libudev is not a standalone library; the only implementation that is one is
# eudev, the split-out subset of systemd.  eudev's release tarballs live on
# git.freedesktop.org, which this build host cannot reach, and the Gentoo
# distfiles mirror carries only 1.6 and 1.7 of it, which are from 2012 and
# predate musl.  So there is no libudev port to write, and lsusb is not built.
#
# libudev is named in DEPENDENCIES so that mk/project-port.mk's flx-require-ports
# stops the build here, naming the port that would have to exist, instead of
# letting meson report a failed pkg-config lookup for a library nobody had said
# was needed.  Both names are real requirements: lsusb really does use udev, in
# lsusb.c and names.c, to enumerate devices and read their properties.
#
# To build this, add a sysutils/eudev port and put it on DEPENDENCIES.  Nothing
# else here needs to change: the meson cross file is written with the right
# quoting, the right host_machine keys and $(FLX_LINK_FLAGS), and the port
# stopped being a build-system problem some time ago.

NAME    := usbutils
VERSION := 018
CATEGORY:= sysutils
LICENSE := GPL-2.0
DEPENDENCIES:= libusb, libudev
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), libusb (sysutils/libusb)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= lsusb
INSTALL_RELPATH:= usr/bin/lsusb

LIBUSB_PREFIX = $(FREELINX_BUILD_DIR)/deps/libusb

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/usbutils-$(VERSION)
BUILD_DIR = $(FREELINX_BUILD_DIR)/work/$(NAME)/build
PROJECT_BIN = $(BUILD_DIR)/lsusb
BUILD_BIN   = $(PROJECT_BIN)

MESON_CROSS = $(FREELINX_BUILD_DIR)/work/$(NAME)/cross.ini

# No -Dusbhid=false.  usbutils 018 dropped the option: usbhid-dump is a plain
# unconditional target now, and passing the old switch stops the configure with
#
#   meson.build:4:0: ERROR: Unknown options: "usbhid"
#
# which names the project's first line and not the switch.

# Machine-file values take single quotes only.  Meson does not parse an ini file
# with an ini parser and reads a double-quoted value as a broken one:
#
#   ERROR: Malformed value in machine file variable 'clang': Double quotes are
#   not supported. Use single quotes.
#   "/home/kanan/FreeLinX/TestForBase/toolchain/bin/clang
#
# so every value below is quoted with '.  Each flag is its own element of its
# list, because meson hands an element to the compiler as a single argument:
# '$(A) $(B)' in one element arrives as one argument with a space inside it,
# --sysroot is swallowed and the build quietly compiles glibc headers.  That is
# check-port-makefiles.py's third check, and meson's machine file is the third
# place it has turned up after a meson *_args list and a CMake linker-flag
# string.
MESON_C_ARGS = -O2 -I$(LIBUSB_PREFIX)/include/libusb-1.0
MESON_C_LIBS = -L$(LIBUSB_PREFIX)/lib

# c_link_args is $(FLX_LINK_FLAGS): --target and --sysroot and -nostdlib and the
# musl startup objects, then -static -fuse-ld=lld.  The list this replaces held
#
#     c_link_args = ["-static", "-fuse-ld=lld", "--rtlib=compiler-rt", "-L..."]
#
# which has no --sysroot and no -nostdlib, so meson linked with clang's
# defaults: the build host's /usr/lib64/crt1.o and crti.o won the search, GCC's
# crtbegin.o and crtend.o came in, and the binary carried both toolchains.
#
#   usr/bin/lsusb
#       mixed build: GCC: (GNU) 16.1.1 alongside clang
#
# The quoting, which meson's machine-file parser is particular about twice over.
# Values take single quotes, never double:
#
#   ERROR: Malformed value in machine file variable 'clang': Double quotes are
#   not supported. Use single quotes.
#
# and list elements are comma-separated.  Space-separated looks right and is not:
#
#   ERROR: Malformed value in machine file variable 'c_args': Expecting rbracket
#   got string.
#
# So one 'word', comma, per argument.
MESON_QUOTE = $(foreach a,$(1),'$(a)',)

# The machine file is rewritten on every configure, never cached.  It names the
# toolchain and the deps prefix by absolute path, so a cached copy is a build
# configured against paths that are no longer there -- and it outlives the run
# that wrote it: meson reports the parse error, leaves the bad file on disk, and
# the next run skips the generate and reports the same error with nothing in the
# log to explain why the file looks the way it does.
PROJECT_CFG_CMDS = \
	mkdir -p "$(BUILD_DIR)" && \
	{ \
		printf '%s\n' '[bin]'; \
		printf "clang = '%s'\n" "$(FREELINX_CC)"; \
		printf "ld = '%s'\n" "$(FREELINX_TOOLCHAIN_BIN)/ld.lld"; \
		printf "ar = '%s'\n" "$(FREELINX_TOOLCHAIN_BIN)/llvm-ar"; \
		printf '%s\n' '' '[host_machine]'; \
		printf "system = 'linux'\n"; \
		printf "cpu_family = 'x86_64'\n"; \
		printf "cpu = 'x86_64'\n"; \
		printf "endian = 'little'\n"; \
		printf '%s\n' '' '[built-in options]'; \
		printf 'c_args = [%s]\n' "$(call MESON_QUOTE,$(MESON_C_ARGS))"; \
		printf 'c_link_args = [%s]\n' "$(call MESON_QUOTE,$(FLX_LINK_FLAGS) $(MESON_C_LIBS))"; \
		printf "prefix = '/usr'\n"; \
	} > "$(MESON_CROSS)" && \
	meson setup "$(BUILD_DIR)" "$(SRC_TREE)" \
		--cross-file "$(MESON_CROSS)" \
		--default-library=static

BUILD_CMDS = \
	ninja -C "$(BUILD_DIR)" -j"$(FREELINX_JOBS)"
