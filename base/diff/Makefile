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

# Members of the OpenBSD source tarball, which unpacks at the repository root
# rather than under usr/src, so the paths carry no usr/src/ prefix.  OpenBSD
# keeps the traditional BSD `usr.bin` name with a literal dot, verified against
# the archive:
#
#   $ tar -tf dist/openbsd-7.9-src.tar.gz | grep diffreg
#   usr.bin/diff/diffreg.c
#
# so a usr/bin/diff/... path finds nothing and the extract fails with
#
#   tar: usr/src/usr.bin/diff/diff.c: Not found in archive
#
# PORT_SRCS below already used the usr.bin spelling; only NETBSD_MEMBERS had the
# NetBSD prefix on it.
NETBSD_MEMBERS = usr.bin/diff/diff.c usr.bin/diff/diff.h \
	usr.bin/diff/diffdir.c usr.bin/diff/diffreg.c \
	usr.bin/diff/xmalloc.c usr.bin/diff/xmalloc.h

# OpenBSD sources assume <sys/cdefs.h> (__dead, __printflike, ...) is pulled
# in transitively; musl has no cdefs.h, so force-include FreeLinX's compat
# stand-in (base/compat/sys/cdefs.h) before every translation unit.
FLX_CPPFLAGS += -include sys/cdefs.h

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/errc.c

# Flat, because mk/base-port.mk extracts NETBSD_MEMBERS with
# --strip-components=2, and usr.bin/diff/diff.c has three components, so what
# lands in $(SRC_DIR) is diff.c.  The same applies to the patch paths below:
# base/chown's patch names a/sbin/chown/chown.c for the same reason.
PORT_SRCS = diff.c diffdir.c diffreg.c xmalloc.c