# FreeLinX/ports - base/nbsvtool : NetBSD 10.1 `/usr/bin/nbsvtool`.
NAME   := nbsvtool
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= nbsvtool
INSTALL_RELPATH:= bin/nbsvtool

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/nbsvtool/nbsvtool.c

PORT_SRCS = usr.bin/nbsvtool/nbsvtool.c

# nbsvtool is a certificate-inspection tool built on OpenSSL.  The library is a
# build product of this tree (build/deps/openssl), the same one bdes links; the
# musl sysroot's own libcrypto.a is a 64-byte stub with no symbols in it.
#
# Added through FLX_LDADD rather than by overriding FLX_LD: base-port.mk already
# has a hook for extra link inputs, and a second copy of the whole link line
# here would be one more place to forget the crt objects and compiler-rt.
OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
FLX_CPPFLAGS += -I$(OPENSSL_PREFIX)/include
FLX_LDADD    += $(OPENSSL_PREFIX)/lib64/libcrypto.a
