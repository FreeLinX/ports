# FreeLinX/ports - base/telnet : NetBSD 10.1 `/usr/bin/telnet`.
#
# Full NetBSD telnet client incl. the libtelnet helper set.  OS porting notes:
#   - `<libtelnet/*.h>` resolves against the extracted lib/libtelnet via
#     `-I$(SRC_DIR)/lib` (members land at lib/libtelnet after --strip 2).
#   - AUTHENTICATION and ENCRYPTION are intentionally NOT defined: the
#     client compiles without the auth/encrypt command tree (authenc.c is
#     empty) and libtelnet's auth.c/encrypt.c/forward.c/pk.c are not built.
#     The unconditional trio misc.c/genget.c/getent.c provides the
#     auth_encrypt_* glue + genget(3) helpers the client always uses.
#   - NetBSD telnet links libterminfo for setupterm(3)/ttytype[]; FreeLinX
#     has no terminfo DB, so base/compat/terminfo_stub.c supplies a
#     setupterm() that always fails - telnet then sends the plain TERM
#     value during TELOPT_TTYPE negotiation (its documented fallback).
NAME   := telnet
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= telnet
INSTALL_RELPATH:= bin/telnet

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# The telnet client + the three unconditional libtelnet sources.
NETBSD_MEMBERS = usr/src/usr.bin/telnet/authenc.c \
	usr/src/usr.bin/telnet/commands.c \
	usr/src/usr.bin/telnet/defines.h \
	usr/src/usr.bin/telnet/externs.h \
	usr/src/usr.bin/telnet/general.h \
	usr/src/usr.bin/telnet/main.c \
	usr/src/usr.bin/telnet/network.c \
	usr/src/usr.bin/telnet/ring.c \
	usr/src/usr.bin/telnet/ring.h \
	usr/src/usr.bin/telnet/sys_bsd.c \
	usr/src/usr.bin/telnet/telnet.c \
	usr/src/usr.bin/telnet/terminal.c \
	usr/src/usr.bin/telnet/types.h \
	usr/src/usr.bin/telnet/utilities.c \
	usr/src/lib/libtelnet/auth.h \
	usr/src/lib/libtelnet/auth-proto.h \
	usr/src/lib/libtelnet/enc-proto.h \
	usr/src/lib/libtelnet/encrypt.h \
	usr/src/lib/libtelnet/genget.c \
	usr/src/lib/libtelnet/getent.c \
	usr/src/lib/libtelnet/misc.c \
	usr/src/lib/libtelnet/misc.h \
	usr/src/lib/libtelnet/misc-proto.h \
	usr/src/include/arpa/telnet.h

PORT_SRCS = usr.bin/telnet/authenc.c \
	usr.bin/telnet/commands.c \
	usr.bin/telnet/main.c \
	usr.bin/telnet/network.c \
	usr.bin/telnet/ring.c \
	usr.bin/telnet/sys_bsd.c \
	usr.bin/telnet/telnet.c \
	usr.bin/telnet/terminal.c \
	usr.bin/telnet/utilities.c \
	lib/libtelnet/genget.c \
	lib/libtelnet/getent.c \
	lib/libtelnet/misc.c

# `<libtelnet/misc.h>` etc. resolve from $(SRC_DIR)/lib/libtelnet/.
FLX_CPPFLAGS += -I$(SRC_DIR)/lib

# setupterm()/ttytype[] stand-in; sigblock/sigsetmask (BSD signal masks);
# reallocarr(3) (NetBSD libc); warnc(3) is declared in flx_bsd.h and
# implemented in sigstub.c.
COMPAT_SRCS = $(FLX_COMPAT)/terminfo_stub.c $(FLX_COMPAT)/sigstub.c \
	$(FLX_COMPAT)/reallocarr.c

# Not portable to a Linux kernel: needs libtelnet, a separate NetBSD library not present in the src set
# Declared rather than left to fail: building it on FreeLinX cannot
# succeed, and an expected failure should not be reported as a regression.
PORT_NOT_PORTABLE:= needs libtelnet, a separate NetBSD library not present in the src set
