# FreeLinX/ports - devel/openssl : OpenSSL 3.3.2.
#
# Real upstream source:
#   https://www.openssl.org/source/openssl-3.3.2.tar.gz
#
# Apache-2.0 licensed.
#
# Produces libcrypto.a and libssl.a.  xorg-server needs one function from it --
# SHA1_Init -- and refuses to configure without some SHA-1, because musl has
# none:
#
#   configure: error: No suitable SHA1 implementation found
#
# The alternatives xorg-server offers are libmd, libsha1, nettle and libgcrypt,
# and libgcrypt is two ports deep (it needs gpg-error) for one function.
# OpenSSL is also what the rest of the tree wants for TLS, so it is worth having
# properly.
#
# Built with OpenSSL's own ./Configure rather than cmake or meson.  The source
# tarball has no CMakeLists.txt -- that build lives in a separate repository --
# and the meson build is not what upstream tests.  The target is
# linux-x86_64, whose only additions to linux-generic64 are -m64 and -DL_ENDIAN:
# it names no libc, so it is correct for a musl target, and the target triple
# and sysroot come in through CC below.
#
# no-shared: everything in FreeLinX links statically, and a .so here would be a
# second libc surface.  no-tests: the test programs are hundreds of host
# binaries this port has no use for.
#
# no-shared means no provider modules: the default, legacy, base and null
# providers are linked into libcrypto itself, which is why there is no
# /usr/lib/ossl-modules content to stage and why the check below looks for the
# provider symbols rather than for module files.

NAME    := openssl
VERSION := 3.3.2
CATEGORY:= devel
LICENSE := Apache-2.0
DEPENDENCIES:= zlib
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), perl (host)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /
DEPS_ONLY := yes
LIBRARY_PORT := yes

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
ZLIB_PREFIX   = $(FREELINX_BUILD_DIR)/deps/zlib

FLX_PROJECT_CXX :=
FLX_PROJECT_LIBS :=

include ../../mk/project-port.mk

PROJECT_LIB = $(SRC_TREE)/build/libcrypto.a
BUILD_BIN   = $(PROJECT_LIB)

# The compiler, target and sysroot go in CC rather than in a flag list, because
# ./Configure writes CC into the generated makefiles as a single word and drops
# anything that arrives some other way.  ZLIB_INCLUDE_DIR and ZLIB_LIBRARY are
# named for the same reason Configure has no other way to find them, and leaving
# them unset is how a musl OpenSSL ends up linking the host's libz.
PROJECT_CFG_CMDS = \
	rm -rf "$(SRC_TREE)/build" "$(SRC_TREE)/libcrypto.a" && \
	mkdir -p "$(SRC_TREE)/build" && \
	cd "$(SRC_TREE)/build" && \
	CC="$(FREELINX_CC) --target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) $(FREELINX_RESOURCE_FLAGS)" \
	CFLAGS="-O2 -fPIC -I$(ZLIB_PREFIX)/include" \
	LDFLAGS="-static -fuse-ld=lld --rtlib=compiler-rt -L$(ZLIB_PREFIX)/lib" \
	"$(SRC_TREE)/Configure" linux-x86_64 \
		--prefix="$(OPENSSL_PREFIX)" \
		--libdir=lib \
		--openssldir="$(OPENSSL_PREFIX)/ssl" \
		--with-zlib-include="$(ZLIB_PREFIX)/include" \
		--with-zlib-lib="$(ZLIB_PREFIX)/lib/libz.a" \
		no-shared no-tests no-docs

# install_sw needs no INSTALL_TOP: ./Configure was given --prefix, and passing
# INSTALL_TOP on the make line instead does nothing -- openssl's generated
# Makefile does not read it -- so the install reaches for /usr/include and stops
# at "cp: cannot create regular file '/usr/include/openssl/aes.h': Permission
# denied".
#
# install_sw leaves the provider modules, the engines, the headers and the
# pkg-config files under $(OPENSSL_PREFIX); only the two archives are taken from
# the build directory, under the names a consumer's -l expects.
BUILD_CMDS = \
	cd "$(SRC_TREE)/build" && \
	make -j"$(FREELINX_JOBS)" && \
	make install_sw

install: all
	$(call flx-require-archive,$(PROJECT_LIB))

	@$(INSTALL) -m 644 "$(PROJECT_LIB)" "$(OPENSSL_PREFIX)/lib/libcrypto.a"
	@$(INSTALL) -m 644 "$(SRC_TREE)/build/libssl.a" "$(OPENSSL_PREFIX)/lib/libssl.a"
	@if [ ! -d "$(OPENSSL_PREFIX)/include/openssl" ]; then \
		printf '[FreeLinX/ports][error] %s: no headers were installed\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@if [ ! -f "$(OPENSSL_PREFIX)/lib/pkgconfig/libcrypto.pc" ]; then \
		printf '[FreeLinX/ports][error] %s: no libcrypto.pc was installed\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@# no-shared means the providers are linked into libcrypto rather than
	@# dlopen()ed, so lib/ossl-modules is empty and that is correct.  Check the
	@# symbols are actually in there rather than the directory: a libcrypto with
	@# no provider in it exits with "no default provider" at the first call, and
	@# it is a link-time mistake rather than a run-time one.
	@if ! $(FREELINX_TOOLCHAIN_BIN)/llvm-nm --defined-only "$(PROJECT_LIB)" | grep -q ossl_default_provider; then \
		printf '[FreeLinX/ports][error] %s: libcrypto.a has no default provider\n' "$(NAME)" >&2; \
		exit 1; \
	fi
	@printf '[FreeLinX/ports] staged libcrypto.a and libssl.a into %s\n' "$(OPENSSL_PREFIX)"
