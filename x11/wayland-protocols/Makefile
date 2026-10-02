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

# Single quotes throughout, because meson does not parse a machine file with an
# ini parser and reads a double-quoted value as a broken one:
#
#   ERROR: Malformed value in machine file variable 'clang': Double quotes are
#   not supported. Use single quotes.
#   "/home/kanan/FreeLinX/TestForBase/toolchain/bin/clang
#
# List elements are comma-separated; space-separated looks right and is not:
#
#   ERROR: Malformed value in machine file variable 'c_link_args': Expecting
#   rbracket got string.
#
# c_link_args is $(FLX_LINK_FLAGS): --target and --sysroot and -nostdlib and the
# musl startup objects, then -static -fuse-ld=lld.  The list this replaces had
# none of the first three, so a link here would take the build host's crt objects
# and leave GCC's crtbegin.o in the binary.
#
# Rewritten on every configure rather than cached: the file names the toolchain
# by absolute path, and it also outlives the run that wrote it, so a parse error
# would be reported again with nothing in the log to explain the file.
MESON_QUOTE = $(foreach a,$(1),'$(a)',)

PROJECT_CFG_CMDS = \
	{ \
		printf '%s\n' '[bin]'; \
		printf "clang = '%s'\n" "$(FREELINX_CC)"; \
		printf "ld = '%s'\n" "$(FREELINX_LD)"; \
		printf "ar = '%s'\n" "$(FLX_HOST_AR)"; \
		printf '%s\n' '' '[host_machine]'; \
		printf "system = 'linux'\n"; \
		printf "cpu_family = 'x86_64'\n"; \
		printf "cpu = 'x86_64'\n"; \
		printf "endian = 'little'\n"; \
		printf '%s\n' '' '[built-in options]'; \
		printf 'c_args = [%s]\n' "$(call MESON_QUOTE,-O2)"; \
		printf 'c_link_args = [%s]\n' "$(call MESON_QUOTE,$(FLX_LINK_FLAGS))"; \
		printf "prefix = '%s'\n" "$(WLPROTO_PREFIX)"; \
	} > "$(MESON_CROSS)" && \
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
