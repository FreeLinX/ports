# FreeLinX/ports - base/ed : NetBSD 10.1 `/bin/ed`.
#
# Builds the ACTUAL NetBSD ed source (bin/ed/{buf,cbc,glbl,io,main,re,sub,
# undo}.c + ed.h from the 10.1 src set) with the FreeLinX toolchain (clang +
# LLD + musl sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() come from base/compat/getprogname.c.
#   - undo.c uses reallocarr(3), a NetBSD libc interface musl lacks; it comes
#     from base/compat/reallocarr.c.
#   - Everything else (regex.h/regcomp, termios, pwd, stdio...) is POSIX and
#     served by musl.
NAME   := ed
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/ed -> src/rootfs/bin/ed.
INSTALL_BIN:= ed
INSTALL_RELPATH:= bin/ed

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/ed/buf.c usr/src/bin/ed/cbc.c \
	usr/src/bin/ed/glbl.c usr/src/bin/ed/io.c \
	usr/src/bin/ed/main.c usr/src/bin/ed/re.c \
	usr/src/bin/ed/sub.c usr/src/bin/ed/undo.c \
	usr/src/bin/ed/ed.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c

PORT_SRCS = bin/ed/buf.c bin/ed/cbc.c bin/ed/glbl.c bin/ed/io.c \
	bin/ed/main.c bin/ed/re.c bin/ed/sub.c bin/ed/undo.c