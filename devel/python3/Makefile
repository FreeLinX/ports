# FreeLinX/ports - devel/python3 : Python 3.13.16 (upstream project port).
#
# Real upstream source:
#   https://www.python.org/ftp/python/3.13.16/Python-3.13.16.tgz
# Built statically with FreeLinX clang + LLD + musl.
#
# PSF-2.0 licensed.  Delivers the python3 interpreter plus the stdlib it needs
# at runtime: staging/usr/bin/python3 (+ the python3.13 alias) and the
# staging/usr/lib/python3.13 tree.
#
# Static-first like the tree: MODULE_BUILDTYPE=static in the configure
# environment builds every detected stdlib extension *into* libpython instead
# of as .so files, because a statically linked musl binary cannot dlopen()
# loadable modules.  --enable-shared=no (the default, spelled out) keeps
# libpython an archive.  --enable-ipv6 is spelled out (same default) because
# the getaddrinfo-bug probe cannot run when cross-compiling and errors out
# unless IPv6 support is decided explicitly; musl's getaddrinfo is fine.
# ac_cv_file__dev_ptmx/ptc are preset for the same reason: device probes
# cannot run cross, so configure demands the answers (Linux: ptmx yes,
# ptc no) instead of guessing.  No PGO/LTO (instrumented binaries, extra runs) and
# nothing that needs the network: plain -O2.
#
# Cross-compiling CPython requires a build interpreter of the SAME version to
# freeze modules (the marshal format differs per version), and the build host
# carries python 3.11.  So PROJECT_CFG_CMDS works in two stages:
#   1. a pristine copy of this source is configured for the BUILD host with the
#      host cc (no FreeLinX flags, real pkg-config, empty PKG_CONFIG_LIBDIR so
#      the tree's musl-target .pc files cannot leak into a host build) and built
#      in full: ensurepip runs under this interpreter at install time and
#      imports subprocess, which needs the _posixsubprocess extension that a
#      bare `make python' never builds;
#   2. the real tree is cross-configured with --with-build-python pointing at it.
# The host copy is skipped when its ./python already exists, so re-running
# do-config only re-runs the (cheap, idempotent) cross configure.
#
# Module wiring (every dep staged under build/deps/ by its own port):
#   _ssl/_hashlib  --with-openssl=$(OPENSSL_PREFIX).  Upstream's macro hardcodes
#                   -L<dir>/lib but this tree's openssl stages lib64/, so -L lib64
#                   rides in LDFLAGS (same shape as devel/libssh2).
#   _sqlite3       sqlite3.pc via pkg-config (sqlite 3.45.3 >= 3.15.2).
#   _ctypes        libffi.pc via pkg-config (3.13 has no bundled libffi left).
#   pyexpat        --with-system-expat + -lexpat (else the bundled copy builds).
#   readline       --with-readline=editline: libedit.pc via pkg-config, whose
#                  Libs.private (-ltermcap) resolves against netbsd-curses.
#   _curses/_panel ncursesw.pc/panelw.pc via pkg-config (netbsd-curses).
#   zlib           zlib.pc via pkg-config; libz.a is also named in LIBS so the
#                  final static link satisfies libcrypto's compression refs.
# _decimal builds against its BUNDLED libmpdec (no system mpdecimal in tree;
# that is what needed the compiler-rt builtins below).  _uuid (libuuid),
# _tkinter, gdbm/dbm, _bz2, _lzma have no port in this tree's build/deps and
# stay skipped; decimal needs no fallback.  --disable-test-modules keeps the
# _test* modules (which need dlopen) out of a static binary.
#
# ensurepip: --with-ensurepip=install (bundled wheels only, no network; pip can
# still come later as its own port if wanted).  The install runs under the
# build interpreter with --root, which lands pip under the host prefix inside
# DESTDIR; stage-extra relocates it into the staged stdlib (importable pip,
# working `python -m pip`).  No pip3 script is staged (that would be a second
# delivered binary).
#
# patches/patch-openssl-no-threads: the OpenSSL staged under build/deps is a
# no-threads build, and Modules/_ssl.c + _hashopenssl.c hard-error without
# OPENSSL_THREADS.  The patch downgrades that to a #warning (single-threaded
# use stays correct; concurrent TLS/hashing across threads would share a
# lock-free libcrypto).  Rebuilding the tree's openssl with threads would
# remove the caveat and the patch.
#
# ensurepip relocation (no patch -- see stage-extra): cross `make install' runs
# ensurepip under the build interpreter, whose sys.prefix is the throwaway host
# prefix, so --root alone installs pip into destdir/<host-prefix>/...
# (silently useless); ensurepip's CLI has no --prefix to fix that with.
# stage-extra therefore relocates the pip package into the staged stdlib's
# site-packages (importable `pip`, working `python -m pip`); the pip3 scripts
# stay behind, since staging undeclared binaries is not this port's job.

NAME    := python3
VERSION := 3.13.16
CATEGORY:= devel
LICENSE := PSF-2.0
DEPENDENCIES:= libffi expat openssl sqlite zlib libedit netbsd-curses
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), host cc (build interpreter)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= python3
INSTALL_RELPATH:= usr/bin/python3
# python3.13 is the same binary under its versioned name.  Symlink, not a copy:
# the INSTALL_ALIASES convention in mk/install.mk (cf. base/gzip, devel/byacc).
INSTALL_ALIASES:= python3.13=python3
# install-rootfs copies INSTALL_RELPATH and nothing else, so the stdlib tree
# the interpreter needs at runtime is named here (the loop does cp -a, which
# handles a directory).
INSTALL_EXTRA_RELPATHS:= usr/lib/python3.13

FFI_PREFIX = $(FREELINX_BUILD_DIR)/deps/libffi
EXPAT_PREFIX = $(FREELINX_BUILD_DIR)/deps/expat
OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
SQLITE_PREFIX = $(FREELINX_BUILD_DIR)/deps/sqlite
ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib
LIBEDIT_PREFIX = $(FREELINX_BUILD_DIR)/deps/libedit
CURSES_PREFIX = $(FREELINX_BUILD_DIR)/deps/netbsd-curses

# Python is C; do NOT pull in the C++/libc++ link libraries.
FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/Python-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/python
BUILD_BIN   = $(PROJECT_BIN)

# Pristine copy for the native build interpreter, kept out of SRC_TREE so the
# cross tree never sees host objects.  `=` (recursive): FREELINX_BUILD_DIR is
# only bound during project-port.mk's include, like ZLIB_PREFIX in base/zlib.
HOST_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/build-python/Python-$(VERSION)
HOST_PYTHON = $(HOST_TREE)/python
HOST_PREFIX = $(FREELINX_BUILD_DIR)/work/$(NAME)/build-python/host-install
# Throwaway DESTDIR for `make install': the port stages only the binary (via
# install.mk) and the stdlib tree (below), never the host filesystem.
TMPINST = $(FREELINX_BUILD_DIR)/work/$(NAME)/destdir-tmp
# 3.13.16 -> 3.13: the versioned stdlib directory.
PY_VER_MAJMIN = $(basename $(VERSION))

# clang compiler-rt builtins, in place of the libgcc a GCC toolchain would add
# (same archive mk/base-port.mk resolves, but preferring the *-musl layout:
# this toolchain ships both gnu and musl variants and the first-sorted gnu one
# is host-built).  The bundled libmpdec needs __udivti3/__umodti3, and with
# -nostdlib clang does not auto-link its rtlib.  Placement matters: the
# archive must sort AFTER libmpdec.a on the final link, and the only slot at
# the very end is SYSLIBS ($(LIBM) $(LIBC)), so it rides --with-libm.
# LDFLAGS/LIBS/MODLIBS all sort too early (archives resolve left to right).
PY_COMPILER_RT = $(firstword $(wildcard $(FREELINX_RESOURCE_DIR)/lib/*musl*/libclang_rt.builtins*.a) $(wildcard $(FREELINX_RESOURCE_DIR)/lib/*/libclang_rt.builtins*.a))

PY_DEPS_CFLAGS = -I$(FFI_PREFIX)/include -I$(EXPAT_PREFIX)/include -I$(OPENSSL_PREFIX)/include -I$(SQLITE_PREFIX)/include -I$(ZLIB_PREFIX)/include -I$(LIBEDIT_PREFIX)/include -I$(LIBEDIT_PREFIX)/include/editline -I$(CURSES_PREFIX)/include
PY_DEPS_LDFLAGS = -L$(FFI_PREFIX)/lib -L$(EXPAT_PREFIX)/lib -L$(OPENSSL_PREFIX)/lib64 -L$(SQLITE_PREFIX)/lib -L$(ZLIB_PREFIX)/lib -L$(LIBEDIT_PREFIX)/lib -L$(CURSES_PREFIX)/lib

PROJECT_CFG_CMDS = \
	if [ ! -x "$(HOST_PYTHON)" ]; then \
		rm -rf "$(HOST_TREE)" && mkdir -p "$(dir $(HOST_TREE))" && cp -a "$(SRC_TREE)" "$(HOST_TREE)" && \
		cd "$(HOST_TREE)" && \
		CC="cc" AR="ar" RANLIB="ranlib" CFLAGS="-O2" CPPFLAGS="" LDFLAGS="" LIBS="" PKG_CONFIG="/usr/bin/pkg-config" PKG_CONFIG_LIBDIR="" PKG_CONFIG_PATH="" ./configure --prefix="$(HOST_PREFIX)" --disable-shared --disable-test-modules --without-ensurepip && \
		make -j"$(FREELINX_JOBS)" CC="cc" AR="ar" RANLIB="ranlib" CFLAGS="-O2" CPPFLAGS="" LDFLAGS="" LIBS=""; \
	fi && \
	cd "$(SRC_TREE)" && \
	test -n "$(PY_COMPILER_RT)" || { printf '[FreeLinX/ports][error] %s: no compiler-rt builtins archive under %s\n' "$(NAME)" "$(FREELINX_RESOURCE_DIR)" >&2; exit 1; } && \
	MODULE_BUILDTYPE=static \
	ac_cv_file__dev_ptmx=yes ac_cv_file__dev_ptc=no \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--build=$(shell "$(SRC_TREE)/config.guess") \
		--prefix=/usr \
		--enable-shared=no \
		--enable-ipv6 \
		--with-libm="$(PY_COMPILER_RT) -lm" \
		--with-system-expat \
		--with-openssl="$(OPENSSL_PREFIX)" \
		--with-openssl-rpath=no \
		--with-readline=editline \
		--with-ensurepip=install \
		--disable-test-modules \
		--with-build-python="$(HOST_PYTHON)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2 $(PY_DEPS_CFLAGS)" \
		CPPFLAGS="$(PY_DEPS_CFLAGS)" \
		LDFLAGS="$(FLX_LINK_FLAGS) $(PY_DEPS_LDFLAGS)" \
		LIBS="$(ZLIB_PREFIX)/lib/libz.a"

# PY_LDFLAGS in the generated Makefile is $(CONFIGURE_LDFLAGS) $(LDFLAGS) and
# PY_CFLAGS contains $(CFLAGS), but the Makefile assigns neither LDFLAGS nor
# CFLAGS itself -- so both arrive a second time from the exported FreeLinX
# environment this framework puts around every build (the musl crt objects
# twice: "duplicate symbol: _start").  CONFIGURE_* already carry the full
# cross flags from configure above, so the build-time environment copies are
# cleared on the make command line.  (The same leak broke the host
# interpreter build earlier; there it needed host values instead.)
BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" CFLAGS="" LDFLAGS=""

# install.mk stages $(BUILD_BIN) as usr/bin/python3 plus the alias symlink; the
# stdlib the interpreter needs at runtime is staged here out of a throwaway
# DESTDIR install (never onto the host).  stage-extra, not an install:
# override, which would lose do-build (see the note in mk/install.mk).
.PHONY: stage-extra
stage-extra:
	@set -e; \
	rm -rf "$(TMPINST)"; \
	$(MAKE) -C "$(SRC_TREE)" install DESTDIR="$(TMPINST)" CFLAGS="" LDFLAGS=""; \
	test -d "$(TMPINST)/usr/lib/python$(PY_VER_MAJMIN)" || { printf '[FreeLinX/ports][error] %s: DESTDIR install produced no stdlib tree\n' "$(NAME)" >&2; exit 1; }; \
	if [ -e "$(STAGE_ROOT)/usr/lib" ] && [ ! -d "$(STAGE_ROOT)/usr/lib" ]; then \
		if cmp -s "$(STAGE_ROOT)/usr/lib" "$(FFI_PREFIX)/lib/libffi.a"; then \
			printf '[FreeLinX/ports][warn] %s: removing stray file %s (byte-identical to the libffi.a that devel/libffi mis-stages as INSTALL_RELPATH=usr/lib; see report)\n' "$(NAME)" "$(STAGE_ROOT)/usr/lib"; \
			rm -f "$(STAGE_ROOT)/usr/lib"; \
		else \
			printf '[FreeLinX/ports][error] %s: %s exists and is not a directory; refusing to stage the stdlib tree over it\n' "$(NAME)" "$(STAGE_ROOT)/usr/lib" >&2; \
			exit 1; \
		fi; \
	fi; \
	mkdir -p "$(STAGE_ROOT)/usr/lib"; \
	rm -rf "$(STAGE_ROOT)/usr/lib/python$(PY_VER_MAJMIN)"; \
	cp -a "$(TMPINST)/usr/lib/python$(PY_VER_MAJMIN)" "$(STAGE_ROOT)/usr/lib/"; \
	if [ -d "$(TMPINST)$(HOST_PREFIX)/lib/python$(PY_VER_MAJMIN)/site-packages/pip" ]; then \
		mv "$(TMPINST)$(HOST_PREFIX)/lib/python$(PY_VER_MAJMIN)/site-packages/pip" "$(TMPINST)$(HOST_PREFIX)/lib/python$(PY_VER_MAJMIN)/site-packages"/pip-*.dist-info "$(STAGE_ROOT)/usr/lib/python$(PY_VER_MAJMIN)/site-packages/"; \
		printf '[FreeLinX/ports] relocated %s pip into staged site-packages\n' "$(NAME)"; \
	else \
		printf '[FreeLinX/ports][error] %s: ensurepip left no pip under %s\n' "$(NAME)" "$(TMPINST)$(HOST_PREFIX)/lib/python$(PY_VER_MAJMIN)/site-packages" >&2; \
		exit 1; \
	fi; \
	printf '[FreeLinX/ports] staged %s stdlib into %s\n' "$(NAME)" "$(STAGE_ROOT)/usr/lib/python$(PY_VER_MAJMIN)"
