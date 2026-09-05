# FreeLinX/ports - net/libpcap : libpcap 1.10.5 (packet capture library).
#
# Portable library for network traffic capture. Build dependency for tcpdump.

NAME    := libpcap
VERSION := 1.10.5
CATEGORY:= net
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:=
INSTALL_RELPATH:= lib/libpcap.a

LIBPCAP_PREFIX = $(FREELINX_BUILD_DIR)/deps/libpcap

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/libpcap-$(VERSION)
PROJECT_BIN = $(LIBPCAP_PREFIX)/lib/libpcap.a
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--prefix="$(LIBPCAP_PREFIX)" \
		--enable-static \
		--disable-shared \
		--without-libnl \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" && \
	make install
