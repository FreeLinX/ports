# FreeLinX/ports - base/nc : NetBSD 10.1 `/usr/bin/nc`.
#
# Builds the ACTUAL NetBSD netcat source (usr.bin/nc/{netcat.c,atomicio.c,
# socks.c} from the 10.1 src set) with the FreeLinX toolchain (clang + LLD +
# musl sysroot), static.  Not an invented nc or a host netcat distribution.
#
# libc notes: netcat.c/socks.c use plain sockets + err(3)/getprogname(3);
# the CRYPTO/socks TLS support is #ifdef'd out in the src Makefile and stays
# out here (no libcrypto link).  getprogname(3) from base/compat.
NAME   := nc
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= nc
INSTALL_RELPATH:= bin/nc

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/nc/netcat.c usr/src/usr.bin/nc/atomicio.c \
	usr/src/usr.bin/nc/atomicio.h usr/src/usr.bin/nc/socks.c

# nc uses strtoi/strtou (bounds-checked parsing), errc (explicit-errno
# error reporting), arc4random_uniform (-r random source port) -- all BSD
# libc functions musl omits -- so the FreeLinX compat set is pulled in.
# readpassphrase(3)/getpassfd(3) are BSD-only; socks.c's getproxypass() is
# patched to use musl's getpass(3) and the `__unused` parameter annotation is
# dropped (patches/patch-socks-compat).  The FreeLinX compat err.h adds
# errc/warnc; the compat stdlib.h wrapper declares strtoi/strtou/
# arc4random_uniform; the compat resolv.h wrapper adds b64_ntop(3) (Proxy
# Basic auth) which musl omits.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strtoi.c \
	$(FLX_COMPAT)/arc4random.c $(FLX_COMPAT)/errc.c \
	$(FLX_COMPAT)/b64_ntop.c

PORT_SRCS = usr.bin/nc/netcat.c usr.bin/nc/atomicio.c usr.bin/nc/socks.c