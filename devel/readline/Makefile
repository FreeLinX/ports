# FreeLinX/ports - devel/readline : readline 8.2.13 (line editing).
#
# GNU readline line-editing library. GPL-3.0 license. Autoconf build, static.
# No curses dependency (--without-curses). Used by interactive shell tools.

NAME    := readline
VERSION := 8.2.13
CATEGORY:= devel
LICENSE := GPL-3.0
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= usr/lib

NCURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/ncurses
READLINE_PREFIX = $(FREELINX_BUILD_DIR)/deps/readline

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/readline-$(VERSION)
PROJECT_BIN = $(READLINE_PREFIX)/lib/libreadline.a
BUILD_BIN   = $(PROJECT_BIN)

# --with-curses, and -L for the terminfo library, because libreadline.a needs a
# terminfo library and there is no termcap in this tree:
#
#   $ nm --undefined-only deps/readline/lib/libreadline.a | grep -E '^t(get|puts|goto)'
#   tgetent tgetflag tgetnum tgetstr tgoto tputs
#
# bash_cv_termcap_lib is passed rather than detected.  It is the cache variable
# the whole detection block is wrapped in -- "if test "X$bash_cv_termcap_lib" =
# "X"" -- so naming the library here skips six link tests and gets the answer
# right, and readline.pc records ncursestw instead of a termcap or ncurses this
# tree does not have.  patches/0001-ncursestw.patch teaches its mapping chain the
# name; without it the value is accepted and then turned into -lncurses anyway.
PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix="$(READLINE_PREFIX)" \
		--enable-static \
		--disable-shared \
		--with-curses \
		bash_cv_termcap_lib=libncursestw \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && make install

.PHONY: install
install: all
	@mkdir -p "$(STAGE_ROOT)/usr/lib"
	@$(INSTALL) -m 644 "$(PROJECT_BIN)" "$(STAGE_ROOT)/usr/lib/libreadline.a"
	# readline/readline.h is the include path every consumer uses: bash and
	# anything using libread spell it <readline/readline.h>, so the staged
	# copy has to keep the directory.  The mkdir above only made
	# staging/usr/include, and the install then said
	#   install: cannot create regular file 'staging/usr/include/readline/readline.h':
	# No such file or directory
	@mkdir -p "$(STAGE_ROOT)/usr/include/readline"
	@$(INSTALL) -m 644 "$(READLINE_PREFIX)/include/readline/readline.h" "$(STAGE_ROOT)/usr/include/readline/readline.h"
	@$(INSTALL) -m 644 "$(READLINE_PREFIX)/include/readline/rlconf.h" "$(STAGE_ROOT)/usr/include/readline/rlconf.h"
	@$(INSTALL) -m 644 "$(READLINE_PREFIX)/include/readline/rlstdc.h" "$(STAGE_ROOT)/usr/include/readline/rlstdc.h"
	@printf '[FreeLinX/ports] staged readline\n'
