# FreeLinX/ports - base/msgs : NetBSD 10.1 `/usr/bin/msgs`.
NAME   := msgs
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= msgs
INSTALL_RELPATH:= bin/msgs

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/msgs/msgs.c \
	usr/src/usr.bin/msgs/pathnames.h

PORT_SRCS = usr.bin/msgs/msgs.c

# msgs sizes its prompt via termcap (tgetent/tgetnum) - no-op stubs on
# FreeLinX (no terminfo DB; msgs falls back to its defaults) - and calls
# sigsetmask(3) (compat/sigstub.c).  musl lacks all three.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/sigstub.c \
	$(FLX_COMPAT)/termcap_stub.c
