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

OPENSSL_PREFIX = $(FREELINX_BUILD_DIR)/deps/openssl
FLX_CPPFLAGS += -I$(OPENSSL_PREFIX)/include
FLX_LD          = $(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) \
			-nostdlib -L$(FREELINX_SYSROOT)/lib \
			$(FLX_CRT) \
			$(OBJ_DIR)/*.o \
			$(OPENSSL_PREFIX)/lib64/libcrypto.a \
			-ldl -lpthread \
			-lc $(FLX_COMPILER_RT) \
			$(FLX_CRT_END)
