# FreeLinX/ports - x11/wayland-protocols : Wayland protocol definitions 1.40.
#
# Data-only port: provides the XML protocol description files used by
# wayland-scanner to generate protocol bindings.  No compiled output;
# the meson build installs protocol XML files and the wayland-protocols.xml
# index into $(WLPROTO_PREFIX)/share/wayland-protocols/.
#
# Build system: Meson (data installation only).

NAME    := wayland-protocols
VERSION := 1.40
CATEGORY:= x11
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= meson (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/share/wayland-protocols

WLPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/wayland-protocols

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/wayland-protocols-$(VERSION)
PROJECT_BIN = $(WLPROTO_PREFIX)/share/wayland-protocols/wayland-protocols.xml
BUILD_BIN   = $(PROJECT_BIN)

MESON_CROSS = $(FREELINX_BUILD_DIR)/work/$(NAME)/cross.ini

PROJECT_CFG_CMDS = \
	if [ ! -f "$(MESON_CROSS)" ]; then \
		printf '[bin]\nclang = "%s"\nld = "%s"\nar = "%s"\n\n[host_machine]\nsystem = "linux"\ncpu_family = "x86_64"\n\n[built-in options]\nc_args = ["-O2"]\nc_link_args = ["-static", "-fuse-ld=lld", "--rtlib=compiler-rt"]\nprefix = "%s"\n' "$(FREELINX_CC)" "$(FREELINX_LD)" "$(FLX_HOST_AR)" "$(WLPROTO_PREFIX)" > "$(MESON_CROSS)"; \
	fi && \
	rm -rf flx-build && \
	meson setup flx-build "$(SRC_TREE)" \
		--cross-file="$(MESON_CROSS)" \
		--prefix="$(WLPROTO_PREFIX)" \
		-Dtests=false

BUILD_CMDS = \
	ninja -C flx-build -j"$(FREELINX_JOBS)" && \
	ninja -C flx-build install

.PHONY: install
install:
	@mkdir -p "$(STAGE_ROOT)/usr/share/wayland-protocols"
	@cp -a "$(WLPROTO_PREFIX)/share/wayland-protocols/"*.xml "$(STAGE_ROOT)/usr/share/wayland-protocols/" 2>/dev/null || true
	@for d in "$(WLPROTO_PREFIX)/share/wayland-protocols/"*/; do \
		if [ -d "$$d" ]; then \
			_dn=$$(basename "$$d"); \
			mkdir -p "$(STAGE_ROOT)/usr/share/wayland-protocols/$$_dn"; \
			cp -a "$$d"*.xml "$(STAGE_ROOT)/usr/share/wayland-protocols/$$_dn/" 2>/dev/null || true; \
		fi; \
	done
	@printf '[FreeLinX/ports] staged wayland-protocols\n'
