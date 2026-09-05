# FreeLinX/ports - editors/nvi2 : nvi2 BSD vi clone (2.2.2).
#
# Modern vi clone with wide character support.
# BSD license. CMake build.

NAME    := nvi2
VERSION := 2.2.2
CATEGORY:= editors
LICENSE := BSD-3-Clause
DEPENDENCIES:= netbsd-curses
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), cmake
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= nvi
INSTALL_RELPATH:= usr/bin/nvi

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/nvi2-$(VERSION)
BUILD_DIR = $(FREELINX_BUILD_DIR)/work/$(NAME)/build
PROJECT_BIN = $(BUILD_DIR)/nvi

NCURSES_PREFIX = $(FREELINX_PORTS_ROOT)/staging/usr

PROJECT_CFG_CMDS = \
	mkdir -p "$(BUILD_DIR)" && \
	$(FREELINX_CC) --target=x86_64-linux-musl --sysroot=$(FREELINX_TOOLCHAIN_DIR)/$(FREELINX_TRIPLE) -I$(NCURSES_PREFIX)/include/ncurses -c $(FREELINX_PORTS_ROOT)/dist/musl_compat.c -o $(BUILD_DIR)/musl_compat.o && \
	cd "$(BUILD_DIR)" && \
	CC="$(FREELINX_CC)" \
	CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(NCURSES_PREFIX)/include/ncurses -I$(NCURSES_PREFIX)/include -Wno-int-conversion -Wno-implicit-function-declaration" \
	LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -fuse-ld=lld --rtlib=compiler-rt -L$(NCURSES_PREFIX)/lib -ldb -lpthread $(BUILD_DIR)/musl_compat.o" \
	cmake "$(SRC_TREE)" \
		-DCMAKE_INSTALL_PREFIX=/usr \
		-DCMAKE_BUILD_TYPE=Release \
		-DUSE_WIDECHAR=ON \
		-DUSE_ICONV=OFF \
		-DRESOLV_IN_LIBC=ON \
		-DCURSES_LIBRARY="$(NCURSES_PREFIX)/lib/libcurses.a" \
		-DTERMINFO_LIBRARY="$(NCURSES_PREFIX)/lib/libterminfo.a" \
		-DDB_INCLUDE_DIR="$(NCURSES_PREFIX)/include/db1"

BUILD_CMDS = \
	cd "$(BUILD_DIR)" && \
	cmake --build . -- -j"$(FREELINX_JOBS)"
