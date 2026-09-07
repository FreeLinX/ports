# FreeLinX/ports - x11/tinyx : Xorg server 1.20.14 (tiny/kdrive X server).
#
# The documented FreeLinX tiny X server built from xorg-server's kdrive
# backends (xfbdev).  This provides a minimal X11 server for framebuffer
# display without the full Xorg infrastructure.  Only the kdrive/xfbdev
# server binary is built; all other X server variants are disabled.
#
# This port is structurally complete but the build may be too heavy to
# integrate at this stage — it requires many X11 library dependencies
# (libXfont, fontutil, pixman, etc.) that are not all yet ported.
# The port definition is correct for when those dependencies arrive.
#
# Build system: autoconf + make.

NAME    := tinyx
VERSION := 1.20.14
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:= pixman
BUILD_DEPENDENCIES:= musl, pixman (graphics/pixman), libpci (future), systemd-logind (future), mesa (future), xorg-fonts (future), autoconf (host), automake (host), make (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= X
INSTALL_RELPATH:= usr/bin/X

TINYX_PREFIX = $(FREELINX_BUILD_DIR)/deps/tinyx

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/xorg-server-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/hw/kdrive/xserver
BUILD_BIN   = $(PROJECT_BIN)

PIXMAN_PREFIX = $(FREELINX_BUILD_DIR)/deps/pixman

PROJECT_CFG_CMDS = \
	./autogen.sh --prefix="$(TINYX_PREFIX)" \
		--host=$(FREELINX_TRIPLE) \
		--enable-kdrive \
		--enable-xfbdev \
		--disable-xorg \
		--disable-dmx \
		--disable-xvfb \
		--disable-xnest \
		--disable-xwin \
		--disable-dri \
		--disable-glx \
		--disable-xwayland \
		--disable-unit-tests \
		--with-fontdir=$(TINYX_PREFIX)/share/fonts \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(PIXMAN_PREFIX)/include/pixman-1" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(PIXMAN_PREFIX)/lib" \
		AR="$(FLX_HOST_AR)" \
		RANLIB="$(FLX_HOST_RANLIB)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"

.PHONY: install
install:
	@mkdir -p "$(STAGE_ROOT)/usr/bin"
	@$(INSTALL) -m 755 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/bin/X"
	@printf '[FreeLinX/ports] staged tinyx X server\n'
