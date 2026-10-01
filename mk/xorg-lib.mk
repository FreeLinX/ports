# FreeLinX/ports - mk/xorg-lib.mk : an X.Org autotools library port.
#
# The X.Org client libraries are all the same project wearing different names:
# one or two .c files, an autotools build, a static archive out of a .libs
# directory, headers under include/, a pkg-config file.  Writing that recipe out
# twenty-five times is twenty-five places for the same bug to hide, and it did.
#
# So a port declares what is actually different about it and includes this:
#
#   NAME    := libXfixes
#   VERSION := 6.0.1
#   XORG_LIB_DEPS := libX11 libXext
#
# and gets the configure line, the -I/-L list, the archive wildcard, the
# ar-archive check and the staging.  Anything genuinely unusual the port adds on
# top of this.
#
# The knobs, all of which a port sets before the include:
#
#   XORG_LIB_DEPS      other FreeLinX ports this one links
#   XORG_LIB_CPPFLAGS  extra -I/-D for headers not under a PREFIX_
#   XORG_PC_NAME       the pkg-config module name, when it is not NAME
#   XORG_PC_FILE       the file to look for, when it differs from the module name
#   XORG_PC_ALIASES    further names to install that file under
#   XORG_ARCHIVE       the archive's name, when it is not NAME
#   XORG_EXTRA_LIBS    further archives to stage beside the first
#   XORG_EXTRA_LDFLAGS extra -L for a library that links itself while building
#   XORG_BUILD_LDFLAGS libtool flags for the build step only
#   XORG_BUILD_LIBS    -l flags for the library's own build-time programs
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
# xcb-util-renderutil xcb-renderutil.pc.  A port that does not set this gets
# $(NAME).pc, which is right for libXfixes and wrong for the rest, and the next
# port's configure then fails with "Package requirements (ice >= 1.1.0 ...) were
# not met".
XORG_PC_NAME ?= $(NAME)

# The name to look for, when it is not the name to install it as.
XORG_PC_FILE ?= $(XORG_PC_NAME)

# Further names to install the same file under, for the one library that answers
# to two: libXfont2's configure asks for "fontenc" and xorg-server asks for
# "xfont2", and the file upstream generates is only one of them.
XORG_PC_ALIASES ?=

# The archive's name, for the same reason.  libXaw builds two of them,
# libXaw6.a and libXaw7.a, and the one consumers want is the second; the
# xcb-util packages name theirs libxcb-keysyms.a, libxcb-icccm.a and so on.
XORG_ARCHIVE ?= $(NAME)

# Further archives to stage beside the first: libXaw's libXaw6.a.
XORG_EXTRA_LIBS ?=

# Extra -L for a library that links itself by name while it is being built:
# libXfont2 builds a helper that links -lXfont2, and without this it stops at
# "ld.lld: error: unable to find library -lXfont2".
XORG_EXTRA_LDFLAGS ?=

# libtool's -all-static, which a library with a noinst_PROGRAMS build tool needs
# and configure must not see.  See x11/libXt for the whole story.
XORG_BUILD_LDFLAGS ?=

# Extra -l for a library's own build-time programs.  The port names them because
# the -l name is not derivable from the port name.
#
# Plain flags, no --start-group.  libtool reorders LDFLAGS and rewrites
# -Wl,--start-group into a bare --start-group before handing it to the
# compiler, and clang rejects the bare form:
#
#   clang: error: unknown argument: '--start-group'
XORG_BUILD_LIBS ?=

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
XORG_LINK_FLAGS = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt $(XORG_LDFLAGS) $(XORG_EXTRA_LDFLAGS)

# The archive is found rather than guessed at, because there are five layouts
# in this tree for one kind of build: src/.libs/ (most), the top-level .libs/
# (libXfont2), the bare source directory, and a per-component subdirectory --
# xcb-util-keysyms puts its archive in keysyms/.libs, xcb-util-wm's in
# icccm/.libs.  A list of those four guesses missed the fifth and the install
# said
#
#   [FreeLinX/ports][error] xcb-util-keysyms: no archive at ""
#
# while the archive sat there under keysyms/.libs.
#
# find, not wildcard, because wildcard cannot descend.  This runs when the
# recipe is expanded -- after the build -- so the archive exists by then.
PROJECT_LIB = $(shell find "$(SRC_TREE)" -name '$(XORG_ARCHIVE).a' 2>/dev/null | head -1)
BUILD_BIN   = $(PROJECT_LIB)

# --prefix is the staged tree, not /usr.  configure writes it into the generated
# .pc files, and a .pc whose prefix is /usr has its Cflags dropped by pkg-config
# as a system path: "Cflags: -I/usr/include" contributes nothing, and a consumer
# that relies on the module for its include path compiles against whatever the
# host has.  That is how xorg-server ended up with -Ideps/xproto/include and no
# xorgproto at all, and then could not compile: XKBproto.h uses _X_NONSTRING
# from the Xfuncproto.h it never found.
#
# PKG_CONFIG_LIBDIR, not PKG_CONFIG_PATH: PATH *adds* to pkg-config's built-in
# list, which on this host contains /usr/lib/pkgconfig and therefore the host's
# libbsd-overlay.pc, whose -isystem /usr/include/bsd puts glibc headers into a
# musl build.  The value itself comes from common.mk, which puts xorgproto's
# directory first on purpose.
PROJECT_CFG_CMDS = \
	./configure \
		--prefix="$(FREELINX_BUILD_DIR)/deps/$(NAME)" \
		--host=$(FREELINX_TRIPLE) \
		--enable-static --disable-shared \
		PKG_CONFIG="$(FLX_PKG_CONFIG)" \
		ac_cv_path_PKG_CONFIG="$(FLX_AC_PATH_PKG_CONFIG)" \
		ac_cv_path_ac_pt_PKG_CONFIG="$(FLX_AC_PATH_AC_PT_PKG_CONFIG)" \
		PKG_CONFIG_LIBDIR="$(FREELINX_PKGCONFIG_LIBDIR)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 -fPIC $(XORG_CPPFLAGS) $(XORG_LIB_CPPFLAGS)" \
		LDFLAGS="$(XORG_LINK_FLAGS)"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" LDFLAGS="$(XORG_LINK_FLAGS) $(XORG_BUILD_LDFLAGS)" \
		LIBS="$(XORG_BUILD_LIBS)"

install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@mkdir -p "$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/pkgconfig" \
		"$(FREELINX_BUILD_DIR)/deps/$(NAME)/include"
	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/$(XORG_ARCHIVE).a"
	@for e in $(XORG_EXTRA_LIBS); do \
		$(INSTALL) -m 644 "$$e" "$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/"; \
	done
	@# The headers are under include/ in most of these and under src/ in the
	@# xcb-util packages, which have no include directory at all -- so copying
	@# include/ alone stops at "cp: cannot stat '.../include/.': No such file or
	@# directory" and the headers never arrive.  Both are searched, and neither
	@# being present is an error rather than a shrug.
	@#
	@# The src/ branch takes headers only, because src/ is also where these
	@# packages build and a plain cp -R brings the object files, the libtool
	@# archive and the generated Makefile in with them.  The comment cannot sit
	@# inside the branch: a shell comment swallows the backslash-newline, and
	@# the else after it is commented out --
	@#
	@#   /bin/sh: -c: line 5: syntax error: unexpected end of file from `if'
	@# The headers are under include/ in most of these, and not at all in the
	@# five xcb-util packages: their sources are in per-component subdirectories
	@# -- keysyms/, icccm/, image/, renderutil/, and src/ for xcb-util itself --
	@# and each installs them as $(includedir)/xcb/<name>.h.  So a copy of
	@# include/ alone says
	@#
	@#   [FreeLinX/ports][error] no include/ or src/ to take headers from
	@#
	@# and a copy of src/ alone brings the object files, the libtool archive and
	@# the generated Makefile in with it.
	@#
	@# The comment cannot go inside the if: a shell comment swallows the
	@# backslash-newline that continues the command, and the else after it is
	@# commented out --
	@#
	@#   /bin/sh: -c: line 5: syntax error: unexpected end of file from `if'
	@#
	@# config*.h and the *_private.h headers are skipped: the first is per-build
	@# and the second is not installed.
	@if [ -d "$(SRC_TREE)/include" ]; then \
		cp -R "$(SRC_TREE)/include/." "$(FREELINX_BUILD_DIR)/deps/$(NAME)/include/"; \
	else \
		mkdir -p "$(FREELINX_BUILD_DIR)/deps/$(NAME)/include/xcb"; \
		find "$(SRC_TREE)" -name '*.h' \
			! -name 'config*.h' ! -name '*private*.h' \
			-exec $(INSTALL) -m 644 {} \
				"$(FREELINX_BUILD_DIR)/deps/$(NAME)/include/xcb/" \;; \
	fi
	# find, not $(wildcard): make expands a recipe's variable references before
	# the recipe runs, so a make-time test of the staged tree asks whether the
	# headers are there before the line above has put them there.  That is the
	# same trap as the .la listed ahead of the .a in the PROJECT_LIB wildcard.
	@if [ -z "$$(find "$(FREELINX_BUILD_DIR)/deps/$(NAME)/include" -name '*.h' -print -quit)" ]; then \
		printf '[FreeLinX/ports][error] %s: no headers were staged\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@$(call flx-stage-pc,$(NAME),$(XORG_PC_FILE),$(XORG_PC_NAME))
	@for a in $(XORG_PC_ALIASES); do \
		$(INSTALL) -m 644 "$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/pkgconfig/$(XORG_PC_NAME).pc" \
			"$(FREELINX_BUILD_DIR)/deps/$(NAME)/lib/pkgconfig/$$a.pc"; \
	done
	@printf '[FreeLinX/ports] staged %s.a into %s\n' "$(XORG_ARCHIVE)" "$(FREELINX_BUILD_DIR)/deps/$(NAME)"
