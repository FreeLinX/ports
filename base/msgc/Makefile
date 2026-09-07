# FreeLinX/ports - base/msgc : NetBSD 10.1 `/usr/bin/msgc`.
NAME   := msgc
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= msgc
INSTALL_RELPATH:= bin/msgc

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/msgc/defs.h \
	usr/src/usr.bin/msgc/msgdb.c \
	usr/src/usr.bin/msgc/msgdb.h \
	usr/src/usr.bin/msgc/msgmain.c \
	usr/src/usr.bin/msgc/pathnames.h \
	usr/src/usr.bin/msgc/msgparse.y \
	usr/src/usr.bin/msgc/msgscan.l \
	usr/src/usr.bin/menuc/defs.h \
	usr/src/usr.bin/menuc/mdb.h \
	usr/src/usr.bin/menuc/util.c \
	usr/src/usr.bin/menuc/avl.c

PORT_SRCS = usr.bin/msgc/msgdb.c \
	usr.bin/msgc/msgmain.c \
	usr.bin/msgc/msgparse.c \
	usr.bin/msgc/msgscan.c \
	usr.bin/menuc/util.c \
	usr.bin/menuc/avl.c

# msgparse.y/msgscan.l are bison/flex sources: pre-generate before compiling.
FLX_PREGEN = cd $(SRC_DIR)/usr.bin/msgc && bison -y -d msgparse.y && mv y.tab.c msgparse.c && mv y.tab.h msgparse.h && flex -o msgscan.c msgscan.l
COMPAT_SRCS += $(FLX_COMPAT)/yywrap.c
