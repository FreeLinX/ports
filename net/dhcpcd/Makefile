# FreeLinX/ports - net/dhcpcd : dhcpcd 10.5.2 (DHCP client).
#
# Upstream dhcpcd (roy marples, NetBSD-flavoured) built statically with the
# FreeLinX clang + LLD + musl toolchain. Provides IP configuration via DHCP.
# License: BSD-2-Clause.
#
# dhcpcd uses ./configure (roy marples' build) + make. It needs a writable
# /var/db/dhcpcd, /run (or /var/run) at runtime, all handled by the rootfs.
#
# BUILD DETAIL (why the link flags ride in FLX_PROJECT_CFLAGS):
#   dhcpcd's configure runs its compiler feature test as
#   `$CC $CPPFLAGS $CFLAGS -Wall -Werror _test.c -o _test` -- it does NOT use
#   LDFLAGS in that test link. clang's default link would hand the job to the
#   host GNU ld.bfd, which fails against the musl sysroot (no -lgcc,
#   crtbeginS.o). So the lld/compiler-rt/static switches must be part of the
#   CFLAGS the test sees. Those are link-only flags, harmless during compile.
#   FLX_PROJECT_CFLAGS is folded into the CFLAGS that project-port.mk exports
#   into the child configure/make environment.

NAME   := dhcpcd
VERSION:= 10.5.2
CATEGORY:= net
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= dhcpcd
INSTALL_RELPATH:= bin/dhcpcd

# dhcpcd is C; no C++ link libs.
FLX_PROJECT_LIBS :=
FLX_PROJECT_LDFLAGS  =

# Link-only switches needed by clang against the musl sysroot; folded into the
# exported CFLAGS so dhcpcd's configure feature-test link works.
FLX_PROJECT_CFLAGS := -O2 --rtlib=compiler-rt -fuse-ld=lld -static

FLX_HOST_AR      ?= /usr/bin/ar
FLX_HOST_RANLIB  ?= /usr/bin/ranlib

include ../../mk/project-port.mk

# dhcpcd's build puts the binary at src/dhcpcd (not the source root).
PROJECT_BIN = $(SRC_TREE)/src/dhcpcd
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure --prefix=/ --mandir=/usr/share/man --without-udev

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
