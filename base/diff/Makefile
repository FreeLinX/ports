# FreeLinX/ports - base/diff : BSD `/usr/bin/diff`.
#
# NetBSD 10.1 ships no /usr/bin/diff in its source set (only tests and
# sdiff), so per FreeLinX policy this ports the same-named BSD utility from
# OpenBSD's userland: the classic Bell Labs / Berkeley diff
# (usr.bin/diff/{diff.c,diffdir.c,diffreg.c,xmalloc.c}), BSD/ISC-licensed,
# non-GNU.  Built with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented diff or a host GNU diffutils binary.
#
# OS porting notes:
#   - patches/patch-diff-nopledge compiles out the OpenBSD-only unveil(2)/
#     pledge(2) sandbox in main() (no equivalent syscalls on Linux).
NAME   := diff
VERSION:= cvs (OpenBSD master)
CATEGORY:= base
LICENSE:= ISC/BSD
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= diff
INSTALL_RELPATH:= bin/diff

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/diff/diff.c usr/src/usr.bin/diff/diff.h \
	usr/src/usr.bin/diff/diffdir.c usr/src/usr.bin/diff/diffreg.c \
	usr/src/usr.bin/diff/xmalloc.c usr/src/usr.bin/diff/xmalloc.h

# OpenBSD sources assume <sys/cdefs.h> (__dead, __printflike, ...) is pulled
# in transitively; musl has no cdefs.h, so force-include FreeLinX's compat
# stand-in (base/compat/sys/cdefs.h) before every translation unit.
FLX_CPPFLAGS += -include sys/cdefs.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/errc.c

PORT_SRCS = usr.bin/diff/diff.c usr.bin/diff/diffdir.c \
	usr.bin/diff/diffreg.c usr.bin/diff/xmalloc.c