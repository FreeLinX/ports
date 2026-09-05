# FreeLinX/ports - base/quota : NetBSD 10.1 `/usr/bin/quota`.
NAME   := quota
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= quota
INSTALL_RELPATH:= bin/quota

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/quota/printquota.c \
	usr/src/usr.bin/quota/printquota.h \
	usr/src/usr.bin/quota/quota.c \
	usr/src/usr.bin/quota/quotautil.c \
	usr/src/usr.bin/quota/quotautil.h

PORT_SRCS = usr.bin/quota/printquota.c \
	usr.bin/quota/quota.c \
	usr.bin/quota/quotautil.c

# quota uses libutil dehumanize_number(3) and revoke(2), BSD quota
# arithmetic macros in flx_bsd.h, and the BSD getfsspec(3)/getfsfile(3)
# mount-table family (musl has no fstab.h) - all in compat/.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/sigstub.c \
	$(FLX_COMPAT)/fstab.c
