# FreeLinX/ports - base/passwd : NetBSD 10.1 `/usr/bin/passwd`.
NAME   := passwd
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= passwd
INSTALL_RELPATH:= bin/passwd

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/passwd/extern.h \
	usr/src/usr.bin/passwd/krb5_passwd.c \
	usr/src/usr.bin/passwd/local_passwd.c \
	usr/src/usr.bin/passwd/pam_passwd.c \
	usr/src/usr.bin/passwd/passwd.c \
	usr/src/usr.bin/passwd/yp_passwd.c

PORT_SRCS = usr.bin/passwd/krb5_passwd.c \
	usr.bin/passwd/local_passwd.c \
	usr.bin/passwd/pam_passwd.c \
	usr.bin/passwd/passwd.c \
	usr.bin/passwd/yp_passwd.c

# Not portable to a Linux kernel: needs Kerberos (krb5.h) for krb5_passwd.c, which FreeLinX does not ship
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= needs Kerberos (krb5.h) for krb5_passwd.c, which FreeLinX does not ship
