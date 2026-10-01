# FreeLinX/ports - x11/xcb-proto : XCB protocol descriptions 1.17.0 (data port).
#
# Real upstream source:
#   https://www.x.org/pub/individual/xcb/xcb-proto-1.17.0.tar.xz
#
# Not a library and not a program: the XML descriptions of the X11 protocol
# extensions -- randr.xml, xinerama.xml, and the rest.
#
# This is what makes libxcb build those extensions at all, and the pkg-config
# files below are load-bearing rather than cosmetic:
#
# libxcb's configure only enables an extension when it can find that
# extension's xcb-<name>.pc on PKG_CONFIG_PATH.  Neither xcb-proto 1.17.0 nor
# 1.15.2 ships a pc/ directory, so left alone libxcb builds the core protocol
# only -- and lemonbar, which includes <xcb/randr.h> and <xcb/xinerama.h>,
# stops at the link with a page of
#
#     undefined symbol: xcb_randr_get_screen_resources_current
#
# The .pc files are generated here from the XML that is actually present, so
# there is one source of truth: an extension is offered to libxcb exactly when
# its description shipped.

NAME    := xcb-proto
VERSION := 1.17.0
CATEGORY:= x11
LICENSE := MIT (X11)
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

XCBPROTO_PREFIX = $(FREELINX_BUILD_DIR)/deps/xcb-proto

include ../../mk/project-port.mk

# Nothing to configure and nothing to compile: a tree of XML.
PROJECT_CFG_CMDS := :
BUILD_CMDS := :

install: all
	@printf '[FreeLinX/ports] staging xcb-proto\n'
	@mkdir -p "$(XCBPROTO_PREFIX)/lib/pkgconfig" "$(XCBPROTO_PREFIX)/include/xcb"
	@cp $(SRC_TREE)/src/*.xml "$(XCBPROTO_PREFIX)/include/xcb/"
	@for x in $(SRC_TREE)/src/*.xml; do \
		n=$$(basename $$x .xml); \
		printf 'prefix=%s\nexec_prefix=$${prefix}\nincludedir=$${prefix}/include\n\nName: XCB %s Extension\nDescription: %s extension for XCB\nVersion: %s\nCflags: -I$${includedir}\n' \
			"$(XCBPROTO_PREFIX)" "$$n" "$$n" "$(VERSION)" \
			> "$(XCBPROTO_PREFIX)/lib/pkgconfig/xcb-$$n.pc"; \
	done
	@# xcb-proto.pc comes from the package's own xcb-proto.pc.in, not from a
	@# hand-written file.  libxcb does not read a Cflags line out of it -- it
	@# asks for two variables,
	@#
	@#   pkg-config --variable=xcbincludedir xcb-proto
	@#   pkg-config --variable=pythondir      xcb-proto
	@#
	@# and uses them as the argument list for src/c_client.py, which generates
	@# every extension source file.  A hand-written file with only Cflags leaves
	@# both variables empty, c_client.py is called with no XML path, and the build
	@# dies at
	@#
	@#   File "./c_client.py", line 3388, in <module>
	@#     module = Module(args[0], output)
	@# IndexError: list index out of range
	@#
	@# with no mention of pkg-config anywhere near it.
	@sed -e 's|@prefix@|$(XCBPROTO_PREFIX)|' \
	    -e 's|@exec_prefix@|$${prefix}|' \
	    -e 's|@datarootdir@|$${prefix}/share|' \
	    -e 's|@datadir@|$${datarootdir}|' \
	    -e 's|@xcbincludedir@|$${prefix}/include/xcb|' \
	    -e 's|@PYTHON_PREFIX@||' \
	    -e 's|@pythondir@|$${prefix}/share/python|' \
	    -e 's|@PACKAGE_VERSION@|$(VERSION)|' \
	    "$(SRC_TREE)/xcb-proto.pc.in" > "$(XCBPROTO_PREFIX)/lib/pkgconfig/xcb-proto.pc"
	@if ! grep -q '^xcbincludedir=' "$(XCBPROTO_PREFIX)/lib/pkgconfig/xcb-proto.pc"; then \
		printf '[FreeLinX/ports][error] %s: xcb-proto.pc has no xcbincludedir\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@printf '[FreeLinX/ports] staged %s protocol descriptions and %s pkg-config files\n' \
		"$$(ls "$(XCBPROTO_PREFIX)/include/xcb"/*.xml | wc -l)" \
		"$$(ls "$(XCBPROTO_PREFIX)/lib/pkgconfig" | wc -l)"
