# FreeLinX/ports - mk/xorg-lib.mk : an X.Org autotools library port.
#
# The X.Org client libraries are all the same project wearing different names:
# one or two .c files, an autotools build, a static archive out of a .libs
# directory, headers under include/, a pkg-config file.  Writing that recipe out
# twenty times is twenty places for the same bug to hide, and it did.
#
# So a port declares what is actually different about it and includes this:
#
#   NAME    := libXfixes
#   VERSION := 6.0.1
#   XORG_LIB_DEPS := libX11 libXext
#
# and gets the configure line, the -I/-L list, the archive wildcard, the
# ar-archive check and the staging.  Anything genuinely unusual -- a meson
# build, a library that needs a second -l, a libtool build tool -- the port
# adds on top.
#
# The knobs a port may set before the include:
#
#   XORG_LIB_DEPS     other FreeLinX ports this one links
#   XORG_LIB_CPPFLAGS extra -I/-D for headers that are not under a PREFIX_
#   XORG_PC_NAME      the pkg-config module name, when it is not NAME
#   XORG_PC_FILE      the file to look for, when it differs from the module name
#   XORG_ARCHIVE      the archive's name, when it is not NAME
#   XORG_EXTRA_LIBS   further archives to stage beside the first
#   XORG_BUILD_LDFLAGS libtool flags for the build step only
#
# XORG_LIB_DEPS must be set before the include, because this file assigns a
# PREFIX_ line from it with $(eval), and $(eval) runs when it is read.  That is
# the same parse-time trap install.mk documents: a rule's prerequisite list is
# expanded when make reads the rule, and an $(eval) is read, not deferred.
# FREELINX_BUILD_DIR itself only arrives with mk/common.mk, through the include
# below -- so the $(eval) comes after it.

XORG_LIB_DEPS ?= libX11
XORG_LIB_CPPFLAGS ?=
XORG_LIB_LIBS ?=

# The pkg-config module name is not the library's name in most of these:
# libXau installs xau.pc, libICE ice.pc, libXt xt.pc, libXaw xaw7.pc, and
# libxcvt -- the other way round -- libxcvt.pc.  A port that does not set this
# gets $(NAME).pc, which is right for libXfixes and wrong for the rest, and the
# next port's configure then fails with "Package requirements (ice >= 1.1.0 ...)
# were not met".
XORG_PC_NAME ?= $(NAME)

# The name to look for, when it is not the name to install it as.  libXfont2
# generates xfont2.pc, but its own configure and xorg-server both ask
# pkg-config for "fontenc", so the file has to be installed under that name.
XORG_PC_FILE ?= $(XORG_PC_NAME)

# The archive's name, for the same reason.  libXaw builds two of them,
# libXaw6.a and libXaw7.a, and the one consumers want is the second.
XORG_ARCHIVE ?= $(NAME)

# Further archives to stage beside the first: libXaw's libXaw6.a.
XORG_EXTRA_LIBS ?=

# Every one of these libraries includes protocol headers, and their order is
# not free:
#
#   xorgproto FIRST.  It ships Xfuncproto.h and Xmd.h newer than the xproto
#   7.0.31 the x.org archive still publishes, and Xlib.h wants the newer
#   macros.  With xproto's copy ahead of it the build stops at
#   "fatal error: 'X11/Xfuncproto.h' file not found".
#
# None of these is a library, so they contribute include paths but no -l.
XORG_PROTO_DEPS = xorgproto xproto xextproto xtrans inputproto kbproto renderproto

include ../../mk/project-port.mk
PREFIX_inputproto = $(FREELINX_BUILD_DIR)/deps/inputproto
PREFIX_kbproto = $(FREELINX_BUILD_DIR)/deps/kbproto
PREFIX_renderproto = $(FREELINX_BUILD_DIR)/deps/renderproto
PREFIX_xextproto = $(FREELINX_BUILD_DIR)/deps/xextproto
PREFIX_xorgproto = $(FREELINX_BUILD_DIR)/deps/xorgproto
PREFIX_xproto = $(FREELINX_BUILD_DIR)/deps/xproto
PREFIX_xtrans = $(FREELINX_BUILD_DIR)/deps/xtrans

# One PREFIX_ per dependency, so a port's flag lists are built from a list
# rather than typed out and left to rot.
$(foreach _d,$(XORG_LIB_DEPS) $(XORG_PROTO_DEPS),$(eval PREFIX_$(_d) = $(FREELINX_BUILD_DIR)/deps/$(_d)))

XORG_CPPFLAGS = -I$(PREFIX_xorgproto)/include -I$(PREFIX_xproto)/include -I$(PREFIX_xextproto)/include -I$(PREFIX_xtrans)/include -I$(PREFIX_inputproto)/include -I$(PREFIX_kbproto)/include -I$(PREFIX_renderproto)/include $(foreach _d,$(XORG_LIB_DEPS),-I$(PREFIX_$(_d))/include)
XORG_LDFLAGS  = $(foreach _d,$(XORG_LIB_DEPS),-L$(PREFIX_$(_d))/lib)

# The link flags in one place, because the build step needs them again alongside
# the --all-static that configure must not see.
XORG_LINK_FLAGS = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt $(XORG_LDFLAGS)

# .libs/ before the bare source directory, in both places libtool can put it:
# see the note at the top of this file.  libXfont2 is the one that lands in the
# top-level .libs rather than src/.libs.
PROJECT_LIB = $(firstword $(wildcard \
	$(SRC_TREE)/src/.libs/$(XORG_ARCHIVE).a \
	$(SRC_TREE)/.libs/$(XORG_ARCHIVE).a \
	$(SRC_TREE)/src/$(XORG_ARCHIVE).a \
	$(SRC_TREE)/$(XORG_ARCHIVE).a))
BUILD_BIN   = $(PROJECT_LIB)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		PKG_CONFIG="/usr/bin/pkg-config" \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC $(XORG_CPPFLAGS) $(XORG_LIB_CPPFLAGS)" \
		LDFLAGS="$(XORG_LINK_FLAGS)"

# XORG_BUILD_LDFLAGS exists for libtool's -all-static, which a library with a
# noinst_PROGRAMS build tool needs and configure must not see.  See x11/libXt
# for the whole story.
BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" LDFLAGS="$(XORG_LINK_FLAGS) $(XORG_BUILD_LDFLAGS)"

install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/pkgconfig" \
		"$(FREELINX_BUILD_DIR)/deps/$(NAME)/include"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/$(XORG_ARCHIVE).a"
	@for e in $(XORG_EXTRA_LIBS); do \
		$(INSTALL) -m 644 "$$e" "$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/"; \
	done
	@cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/$(NAME)/include/"
	@$(call flx-stage-pc,$(NAME),$(XORG_PC_FILE),$(XORG_PC_NAME))
	@printf '[FreeLinX/ports] staged %s.a into %s\n' "$(XORG_ARCHIVE)" "$(FREELINX_BUILD_DIR)/deps/$(NAME)"
