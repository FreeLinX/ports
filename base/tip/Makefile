# FreeLinX/ports - base/tip : NetBSD 10.1 `/usr/bin/tip`.
#
# tip is one binary under two names: NetBSD's Makefile has
#   LINKS= ${BINDIR}/tip ${BINDIR}/cu
# and /usr/bin/cu has been a symlink to tip since long before this source was
# written.  INSTALL_ALIASES below stages that link, so a FreeLinX system has cu
# the way every BSD it claims to be derived from does.
NAME   := tip
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= tip
INSTALL_RELPATH:= bin/tip
INSTALL_ALIASES:= cu=tip

include ../../mk/base-port.mk

# Two sources for the dialer side, not one, and the distinction is worth
# keeping straight.  NetBSD's Makefile lists
#
#   SRCS = acu.c acutab.c cmds.c cmdtab.c cu.c hunt.c partab.c remote.c tip.c
#          tipout.c value.c vars.c ${DIALSRCS}
#   DIALSRCS = hayes.c
#   .PATH: ${.CURDIR}/aculib
#
# so hunt.c is an ordinary source and hayes.c is the *extra* dialer, resolved
# out of aculib/.  A build needs both: hunt.c provides hunt() and HW, which
# cu.c:218 and tip.c:214 call to resolve a hardwired entry in acutab, and
# hayes.c provides the hay_dialer/hay_disconnect/hay_abort trio that the HAYES
# entry in acutab names.  With only hunt.c the port fails to link on three
# symbols none of its own sources mention; with only hayes.c it fails on the
# one symbol `hunt'.
#
# The dialer in use is a compile-time choice, made on NetBSD's compile line
# rather than in a header, and every TU reads it - which is why NetBSD's own
# Makefile has `acutab.o remote.o: Makefile', noting that remote.o depends on
# the Makefile precisely because of DEFBR and DEFFS.  These are the hayes
# (HAYES) defaults: 9600 baud, stdio buffer size.
#
# Without them cu.c:84 and remote.c:99/123 fail with DEFBR undeclared, since
# nothing in the source defines it.
FLX_CPPFLAGS += -DDEFBR=9600 -DDEFFS=BUFSIZ -DHAYES

# NetBSD's own compile line is `CPPFLAGS+= -I${.CURDIR}' for the same reason:
# the dialer lives in aculib/ one directory down, and a `#include "tip.h"'
# searches only the including file's own directory before the include path, so
# without this aculib/hayes.c cannot find a header that is plainly there.
FLX_CPPFLAGS += -I$(SRC_DIR)/usr.bin/tip

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/tip/acu.c \
	usr/src/usr.bin/tip/acutab.c \
	usr/src/usr.bin/tip/cmds.c \
	usr/src/usr.bin/tip/cmdtab.c \
	usr/src/usr.bin/tip/cu.c \
	usr/src/usr.bin/tip/aculib/hayes.c \
	usr/src/usr.bin/tip/hunt.c \
	usr/src/usr.bin/tip/partab.c \
	usr/src/usr.bin/tip/pathnames.h \
	usr/src/usr.bin/tip/remote.c \
	usr/src/usr.bin/tip/tip.c \
	usr/src/usr.bin/tip/tip.h \
	usr/src/usr.bin/tip/tipout.c \
	usr/src/usr.bin/tip/value.c \
	usr/src/usr.bin/tip/vars.c

PORT_SRCS = usr.bin/tip/acu.c \
	usr.bin/tip/acutab.c \
	usr.bin/tip/cmds.c \
	usr.bin/tip/cmdtab.c \
	usr.bin/tip/cu.c \
	usr.bin/tip/aculib/hayes.c \
	usr.bin/tip/hunt.c \
	usr.bin/tip/partab.c \
	usr.bin/tip/remote.c \
	usr.bin/tip/tip.c \
	usr.bin/tip/tipout.c \
	usr.bin/tip/value.c \
	usr.bin/tip/vars.c

# cu.c:144 parses the -s flag with strtoi(3).  musl ships no strtoi(3), and
# base-port.mk's defaults do not include it, so it has to be named here.
PORT_COMPAT_SRCS += $(FLX_COMPAT)/strtoi.c

# remote.c resolves a host's capabilities through cgetent(3)/cgetcap(3)/cgetstr(3)
# and sets the terminal through cgetset(3).  getcap.c implements the whole
# cget family, cgetset included.
PORT_COMPAT_SRCS += $(FLX_COMPAT)/getcap.c

# sigstub.c is a grab bag of the small NetBSD libc entry points musl lacks, and
# tip needs four of them, in three different files:
#   tipout.c  sigsetmask(2)   - musl has sigprocmask(2) only
#   cmds.c    closefrom(3)    - three calls, to drop inherited descriptors
#   cu.c      warnc(3)        - reports a bad -s argument
# It is in the defaults for no port that links it, so name it.
PORT_COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
