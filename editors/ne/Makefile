# FreeLinX/ports - editors/ne : the nice editor 3.3.3.
#
# ne is a small, fast, cat-grade configurable text editor (MIT-licensed).
# It builds with a plain makefile.  FreeLinX builds it against the FreeLinX
# clang + LLD + musl toolchain, static.
#
# Build strategy:
#   - NE_ANSI=1 makes ne self-contained (built-in termcap, no external
#     curses/termcap library to link).
#   - ne.h still needs <curses.h> at compile time; the FreeLinX
#     netbsd-curses staging headers (staging/usr/include/ncurses) provide it.
#   - -D_GNU_SOURCE + -D_REGEX_LARGE_OFFSETS are required by ne's use of the
#     glibc-GNU regex extensions (RE_NO_BK_VBAR / RE_NO_EMPTY_RANGES) on the
#     musl regcomp (FreeLinX compat: yes), and -DSTDC_HEADERS/-DHAVE_SNPRINTF
#     match upstream defaults.
#   - LDFLAGS carries --rtlib=compiler-rt for the static musl crt set.

NAME   := ne
VERSION:= 3.3.3
CATEGORY:= editors
LICENSE:= MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), netbsd-curses headers (staging)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ne
INSTALL_RELPATH:= usr/bin/ne

include ../../mk/project-port.mk

# The ne-3.3.3.tar.gz extracts to a top-level ne-3.3.3/ directory; the binary
# is produced in src/.
SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/ne-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/src/ne

NCURSES_INC = $(FREELINX_STAGING_ROOT)/usr/include

PROJECT_CFG_CMDS = :

BUILD_CMDS = \
	cd "$(SRC_TREE)/src" && \
	$(MAKE) NE_ANSI=1 NE_GLOBAL_DIR=/usr/share/ne \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -std=c99 -Wall -Wno-parentheses -fno-strict-aliasing -D_REGEX_LARGE_OFFSETS -D_GNU_SOURCE -DSTDC_HEADERS -DHAVE_SNPRINTF -DNE_TERMCAP -DNE_ANSI -I$(NCURSES_INC)/ncurses -I$(NCURSES_INC)" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(FREELINX_STAGING_ROOT)/usr/lib"