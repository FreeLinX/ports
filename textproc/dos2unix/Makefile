# FreeLinX/ports - textproc/dos2unix : dos2unix 7.5.2 (text file format converter).
#
# BSD-2-Clause licensed converter of text files between DOS/Windows and
# Unix line-ending formats.  Plain POSIX make build (no autoconf), same
# pattern as sysutils/abduco.  CC/CFLAGS/LDFLAGS passed explicitly to make.

NAME    := dos2unix
VERSION := 7.5.2
CATEGORY:= textproc
LICENSE := BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= dos2unix
INSTALL_RELPATH:= usr/bin/dos2unix

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/dos2unix-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/dos2unix

PROJECT_CFG_CMDS = :

BUILD_CMDS = \
	cd "$(SRC_TREE)" && \
	$(MAKE) CC="$(FREELINX_CC)" \
		CFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -O2" \
		LDFLAGS="--target=$(FREELINX_TRIPLE) --sysroot=$(FREELINX_SYSROOT) -static -fuse-ld=lld --rtlib=compiler-rt" \
		dos2unix
