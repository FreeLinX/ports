# FreeLinX/ports - base/bdes : NetBSD 10.1 `/usr/bin/bdes`.
NAME   := bdes
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= bdes
INSTALL_RELPATH:= bin/bdes

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/bdes/bdes.c

PORT_SRCS = usr.bin/bdes/bdes.c
COMPAT_SRCS += $(FLX_COMPAT)/des_sun.c

# des_setkey/des_cipher come from base/compat/des_sun.c, which is backed by
# OpenSSL's DES implementation.  The library is a build product of this tree
# (build/deps/openssl); the musl sysroot's own libcrypto.a is a 64-byte stub
# with no symbols in it, so naming that one would link nothing.
#
# This used to override FLX_LD outright and list its inputs as
# "$(OBJ_DIR)/*.o".  That is the stale-object bug mk/base-port.mk was fixed for
# everywhere else: anything left in the object directory by an earlier build
# went into the binary, and a source dropped from PORT_SRCS kept being linked.
# base-port.mk has a FLX_LDADD hook for exactly this, and it already names the
# objects correctly, so the whole override is unnecessary.
OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
FLX_CPPFLAGS += -I$(OPENSSL_PREFIX)/include
FLX_LDADD    += $(OPENSSL_PREFIX)/lib64/libcrypto.a -ldl -lpthread
