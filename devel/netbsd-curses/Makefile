# FreeLinX/ports - devel/netbsd-curses : NetBSD curses library (0.3.2).
#
# Portable BSD curses library, alternative to ncurses.
# BSD license. GNUmakefile-based build with cross-compilation support.

NAME    := netbsd-curses
VERSION := 0.3.2
CATEGORY:= devel
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_RELPATH:= usr/lib

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/netbsd-curses-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/libcurses/libcurses.a

# Build all libraries: libcurses, libterminfo, libform, libmenu, libpanel
# plus the tic/tput/tset utilities.
PROJECT_CFG_CMDS = \
	printf 'CC = %s\nHOSTCC = cc\nAR = %s\nRANLIB = %s\nPREFIX = /usr\nLIBDIR = $$(PREFIX)/lib\nINCDIR = $$(PREFIX)/include\nCFLAGS = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -fPIC\nLDFLAGS = $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static\n' \
		"$(FREELINX_CC)" "$(FREELINX_AR)" \
		"$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" > "$(SRC_TREE)/config.mak"

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) HOSTCC=cc CC="$(FREELINX_CC)" AR="$(FREELINX_AR)" \
		RANLIB="$(FREELINX_TOOLCHAIN_BIN)/llvm-ranlib" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -fPIC" \
		CPPFLAGS="-DTERMINFO_COMPAT -I. -I./libterminfo" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static" \
		CFLAGS_HOST="-DTERMINFO_COMPAT" \
		PREFIX=/usr \
		libterminfo/libterminfo.a \
		libcurses/libcurses.a \
		libform/libform.a \
		libmenu/libmenu.a \
		libpanel/libpanel.a

# Install headers and static libraries.
.PHONY: install
install: all
	@# Libraries
	@mkdir -p "$(STAGE_ROOT)/usr/lib" 2>/dev/null; true
	@for lib in libcurses/libcurses.a libterminfo/libterminfo.a \
	            libform/libform.a libmenu/libmenu.a libpanel/libpanel.a; do \
		if [ -f "$(SRC_TREE)/$$lib" ]; then \
			$(INSTALL) -m 644 "$(SRC_TREE)/$$lib" "$(STAGE_ROOT)/usr/lib/$$(basename $$lib)"; \
			printf '[FreeLinX/ports] staged %s\n' "$$lib"; \
		fi; \
	done
	@# Headers
	@mkdir -p "$(STAGE_ROOT)/usr/include/ncurses"
	@for h in $(SRC_TREE)/libcurses/*.h $(SRC_TREE)/libterminfo/*.h; do \
		if [ -f "$$h" ]; then \
			$(INSTALL) -m 644 "$$h" "$(STAGE_ROOT)/usr/include/ncurses/$$(basename $$h)"; \
		fi; \
	done
	@# Terminfo database
	@mkdir -p "$(STAGE_ROOT)/usr/share/terminfo"
	@if [ -d "$(SRC_TREE)/terminfo" ]; then \
		cd "$(SRC_TREE)/terminfo" && find . -type f -exec \
			$(INSTALL) -m 644 {} "$(STAGE_ROOT)/usr/share/terminfo/{}" \;; \
	fi
	@printf '[FreeLinX/ports] staged netbsd-curses\n'
