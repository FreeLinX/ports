# FreeLinX/ports - textproc/figlet : figlet 2.2.5 (banner generator).
#
# Classic program for making large letters out of ordinary text.
# 100% non-GNU, static Musl libc + Clang build.

NAME    := figlet
VERSION := 2.2.5
CATEGORY:= textproc
LICENSE := BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= figlet
INSTALL_RELPATH:= usr/bin/figlet

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/figlet-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/figlet
BUILD_BIN   = $(PROJECT_BIN)

BUILD_CMDS = \
	$(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2 -DTLF_FONTS \
		-D__BEGIN_DECLS= -D__END_DECLS= -DDEFAULTFONTDIR='"/usr/share/figlet"' -DDEFAULTFONTFILE='"standard"' \
		-c $(SRC_TREE)/figlet.c $(SRC_TREE)/zipio.c $(SRC_TREE)/crc.c $(SRC_TREE)/inflate.c $(SRC_TREE)/utf8.c && \
	$(FREELINX_CC) $(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld \
		-o $(PROJECT_BIN) figlet.o zipio.o crc.o inflate.o utf8.o
