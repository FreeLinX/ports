# FreeLinX/ports - base/dc : NetBSD 10.1 `/usr/bin/dc`.
#
# Builds the ACTUAL NetBSD dc source (usr.bin/dc/{main.c,dc.c,bcode.c,
# inout.c,mem.c,stack.c} from the 10.1 src set) with the FreeLinX toolchain
# (clang + LLD + musl sysroot), static.  Not an invented dc or a host GNU
# bc/dc binary.
#
# libc notes: dc's arbitrary-precision math comes from <openssl/bn.h>
# (BIGNUM).  musl has no OpenSSL, so dc links the STATIC libcrypto.a produced
# by the FreeLinX openssl port (build/deps/openssl) at build time; the same
# non-GNU, permissively-licensed component FreeLinX already ships.  The
# openssl include dir is added for <openssl/bn.h>/<openssl/err.h>.
NAME   := dc
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= openssl (static libcrypto.a), musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= dc
INSTALL_RELPATH:= bin/dc

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/dc/main.c usr/src/usr.bin/dc/dc.c \
	usr/src/usr.bin/dc/bcode.c usr/src/usr.bin/dc/bcode.h \
	usr/src/usr.bin/dc/inout.c usr/src/usr.bin/dc/mem.c \
	usr/src/usr.bin/dc/extern.h usr/src/usr.bin/dc/stack.c

# OpenSSL headers (BIGNUM) + the FreeLinX openssl port's static libcrypto.a,
# both under the configureable build dir (never hard-coded developer paths).
FLX_CPPFLAGS += -I"$(FREELINX_BUILD_DIR)/deps/openssl/include"
FLX_LDADD    += $(FREELINX_BUILD_DIR)/deps/openssl/lib64/libcrypto.a

# dc calls errc(1, EISDIR, ...) -- explicit-errno err(3) musl lacks -- and
# mem.c's breallocarray() grows its BIGNUM stack with reallocarr(3) (also
# absent from musl; supplied by compat/reallocarr.c with the BSD contract).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/errc.c \
	$(FLX_COMPAT)/reallocarr.c

PORT_SRCS = usr.bin/dc/main.c usr.bin/dc/dc.c usr.bin/dc/bcode.c \
	usr.bin/dc/inout.c usr.bin/dc/mem.c usr.bin/dc/stack.c