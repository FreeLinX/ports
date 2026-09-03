# FreeLinX/ports - base/su : NetBSD 10.1 `su`.
#
# Switch user identity.  NetBSD usr.bin/su source.
# Without PAM support, uses direct crypt/password checking.
NAME   := su
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= su
INSTALL_RELPATH:= bin/su

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/su/su.c usr/src/usr.bin/su/suutil.c usr/src/usr.bin/su/suutil.h
PORT_SRCS = usr.bin/su/su.c usr.bin/su/suutil.c
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/consttime_memequal.c $(FLX_COMPAT)/estrlcpy.c $(FLX_COMPAT)/strlcpy.c $(FLX_COMPAT)/emalloc.c
FLX_CPPFLAGS = -I$(FLX_COMPAT) -I$(SRC_DIR) -DUSE_SHADOW -DNO_PAM -include $(FLX_COMPAT)/su_compat.h
FLX_LDADD = -lcrypt
