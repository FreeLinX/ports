# FreeLinX/ports - devel/byacc : byacc 20260126 (Berkeley Yacc, maintained).
#
# The parser generator every grammar in the tree needs (libedit, python and
# friends all want a yacc at build time). Public domain, deliberately
# compiler-agnostic C: no patches, no GNU code anywhere near it.
# Builds the `yacc` binary; `byacc` is the same program under its own name.

NAME    := byacc
VERSION := 20260126
CATEGORY:= devel
LICENSE := public-domain
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= yacc
INSTALL_RELPATH:= usr/bin/yacc
INSTALL_ALIASES:= byacc=yacc

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/byacc-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/yacc
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	cd "$(SRC_TREE)" && ./configure \
		--prefix=/usr \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) $(FREELINX_RESOURCE_FLAGS) -O2" \
		LDFLAGS="$(FLX_LINK_FLAGS)"

BUILD_CMDS = \
	make -C "$(SRC_TREE)" -j"$(FREELINX_JOBS)" yacc
