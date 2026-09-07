# FreeLinX/ports - base/stty : NetBSD 10.1 `/bin/stty`.
#
# Builds the ACTUAL NetBSD stty utility source (bin/stty/*.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() come from base/compat/getprogname.c.
#   - The terminal settings use POSIX termios (tcgetattr/tcsetattr/cfmakeraw),
#     provided by musl's <termios.h>.
NAME   := stty
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/stty -> src/rootfs/bin/stty.
INSTALL_BIN:= stty
INSTALL_RELPATH:= bin/stty

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/stty/cchar.c usr/src/bin/stty/gfmt.c \
	usr/src/bin/stty/key.c usr/src/bin/stty/modes.c \
	usr/src/bin/stty/print.c usr/src/bin/stty/stty.c \
	usr/src/bin/stty/extern.h usr/src/bin/stty/stty.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = bin/stty/cchar.c bin/stty/gfmt.c bin/stty/key.c \
	bin/stty/modes.c bin/stty/print.c bin/stty/stty.c
