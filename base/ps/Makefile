# FreeLinX/ports - base/ps : NetBSD 10.1 `/bin/ps`.
#
# Process listing utility.  Reads /proc on Linux.  NetBSD source set.
NAME   := ps
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ps
INSTALL_RELPATH:= bin/ps

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/ps/ps.c usr/src/bin/ps/ps.h usr/src/bin/ps/extern.h \
		usr/src/bin/ps/fmt.c usr/src/bin/ps/keyword.c usr/src/bin/ps/print.c \
		usr/src/bin/ps/nlist.c
PORT_SRCS = bin/ps/ps.c bin/ps/fmt.c bin/ps/keyword.c bin/ps/print.c bin/ps/nlist.c
FLX_CPPFLAGS = -I$(FLX_COMPAT) -I$(SRC_DIR) -D_EPOLL -DHAVE_KVM -DHAVE_SWAP
FLX_LDADD =
