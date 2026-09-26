# FreeLinX/ports - base/whatis : NetBSD 10.1 `/usr/bin/whatis`.
NAME   := whatis
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= whatis
INSTALL_RELPATH:= bin/whatis

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/whatis/whatis.c \
	usr/src/usr.bin/man/manconf.h \
	usr/src/usr.bin/man/manconf.c \
	usr/src/usr.bin/man/pathnames.h

# whatis.c calls config(3) to read /etc/man.conf; that function lives in
# man/manconf.c, so it has to be linked in rather than stubbed - it is the code
# that resolves the MANPATH list whatis searches.
PORT_SRCS = usr.bin/whatis/whatis.c \
	usr.bin/man/manconf.c

# whatis.c does #include "manconf.h", which NetBSD keeps in ../man/.
# The member is extracted under $(SRC_DIR)/usr.bin/man, which is not on the
# default include path, so point the compiler at it.
FLX_CPPFLAGS += -I$(SRC_DIR)/usr.bin/man
