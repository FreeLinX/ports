# FreeLinX/ports - security/signify : signify 33 (file signing utility).
#
# Real upstream source:
#   https://codeberg.org/aperezdc/signify/releases/download/v33/signify-33.tar.xz
# Built statically with FreeLinX clang + LLD + musl.
#
# Portable OpenBSD signify: sign and verify file signatures.  ISC license.
# Needs libbsd >= 0.11, provided by devel/libbsd, which publishes
# build/deps/libbsd the way every other library port in this tree does
# (x11/fontconfig names its deps prefixes the same way).  The staging
# tree is where a port leaves what it installs into the rootfs, and a
# -I or -L into it is a -I or -L into the build host's /usr as well as
# ours -- so this port names the deps prefix, never staging/.
#
# Upstream ships `make static-musl', which is EXTRA_CFLAGS/EXTRA_LDFLAGS
# -pthread plus -static with MUSL=1 -- and MUSL=1 forces BUNDLED_LIBBSD=1,
# i.e. upstream's own vendored libbsd copy.  This port imitates the static
# half of that target (-pthread, -static) but links the staged libbsd
# instead: the non-bundled path finds it through pkg-config libbsd-overlay,
# whose .pc files devel/libbsd installs into build/deps.

NAME    := signify
VERSION := 33
CATEGORY:= security
LICENSE := ISC
DEPENDENCIES:= libbsd
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), libbsd (devel/libbsd)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= signify
INSTALL_RELPATH:= usr/bin/signify

# devel/libbsd publishes build/deps/libbsd, like every other library port
# in this tree.
PREFIX_libbsd = $(FREELINX_BUILD_DIR)/deps/libbsd

# signify is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/signify-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/signify
BUILD_BIN   = $(PROJECT_BIN)

# No configure step: upstream builds straight from its own Makefile.
PROJECT_CFG_CMDS = :

# Imitates upstream `make static-musl' (EXTRA -pthread, -static) with the
# FreeLinX compiler and the staged libbsd instead of the bundled copy.
# CC/LD ride on the command line so the upstream Makefile's `?=' defaults
# (musl-gcc) never win; libbsd-overlay comes from pkg-config, which the
# framework points at build/deps via PKG_CONFIG_LIBDIR.
BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" \
		CC="$(FREELINX_CC)" \
		LD="$(FREELINX_CC)" \
		EXTRA_CFLAGS="-pthread" \
		EXTRA_LDFLAGS="-pthread -static"
