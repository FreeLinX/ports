# FreeLinX/ports - base/tail : NetBSD 10.1 `/usr/bin/tail`.
#
# tail's -f follow() uses kqueue(2)/EVFILT_VNODE which does not exist on
# Linux/musl.  forward.c is patched to drop the kqueue machinery and follow by
# plain 1s polling (the code already shipped a USE_SLEEP fallback path, which is
# now the only one compiled), so -f still tracks appended data / file rotation.
# Everything else is self-contained (needs only getprogname from compat).
NAME   := tail
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tail
INSTALL_RELPATH:= bin/tail

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tail/extern.h usr/src/usr.bin/tail/forward.c \
	usr/src/usr.bin/tail/misc.c usr/src/usr.bin/tail/read.c \
	usr/src/usr.bin/tail/reverse.c usr/src/usr.bin/tail/tail.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/tail/forward.c usr.bin/tail/misc.c \
	usr.bin/tail/read.c usr.bin/tail/reverse.c usr.bin/tail/tail.c
