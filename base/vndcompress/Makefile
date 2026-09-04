# FreeLinX/ports - base/vndcompress : NetBSD 10.1 `/usr/bin/vndcompress`.
NAME   := vndcompress
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= vndcompress
INSTALL_RELPATH:= bin/vndcompress

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/vndcompress/common.h \
	usr/src/usr.bin/vndcompress/main.c \
	usr/src/usr.bin/vndcompress/offtab.c \
	usr/src/usr.bin/vndcompress/offtab.h \
	usr/src/usr.bin/vndcompress/utils.c \
	usr/src/usr.bin/vndcompress/utils.h \
	usr/src/usr.bin/vndcompress/vndcompress.c \
	usr/src/usr.bin/vndcompress/vnduncompress.c

PORT_SRCS = usr.bin/vndcompress/main.c \
	usr.bin/vndcompress/offtab.c \
	usr.bin/vndcompress/utils.c \
	usr.bin/vndcompress/vndcompress.c \
	usr.bin/vndcompress/vnduncompress.c
COMPAT_SRCS += $(FLX_COMPAT)/strsuftoll.c
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c

ZLIB_PREFIX = $(FREELINX_BUILD_DIR)/deps/zlib
FLX_CPPFLAGS += -I$(ZLIB_PREFIX)/include
FLX_LD          = $(CC) $(FREELINX_CFLAGS) $(FREELINX_LDFLAGS) \
			-nostdlib -L$(FREELINX_SYSROOT)/lib \
			$(FLX_CRT) \
			$(OBJ_DIR)/*.o \
			$(ZLIB_PREFIX)/lib/libz.a \
			-lc $(FLX_COMPILER_RT) \
			$(FLX_CRT_END)
