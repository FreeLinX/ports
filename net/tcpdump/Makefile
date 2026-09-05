# FreeLinX/ports - net/tcpdump : tcpdump 4.99.5 (packet analyzer).
#
# Network packet analyzer for command line. Depends on libpcap.

NAME    := tcpdump
VERSION := 4.99.5
CATEGORY:= net
LICENSE := BSD-3-Clause
DEPENDENCIES:= libpcap
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), libpcap (net/libpcap)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= tcpdump
INSTALL_RELPATH:= usr/sbin/tcpdump

LIBPCAP_PREFIX = $(FREELINX_BUILD_DIR)/deps/libpcap

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/tcpdump-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/tcpdump
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix=/usr \
		--sbindir=/usr/sbin \
		--enable-static \
		--disable-shared \
		--without-libnl \
		--without-crypto \
		--with-libpcap="$(LIBPCAP_PREFIX)" \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -I$(LIBPCAP_PREFIX)/include" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt -L$(LIBPCAP_PREFIX)/lib"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)"
