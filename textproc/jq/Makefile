# FreeLinX/ports - textproc/jq : jq 1.7.1 (command-line JSON processor).
#
# MIT-licensed lightweight and flexible command-line JSON processor.
# Autoconf configure, static only.  Links against -lm (math library from
# musl, provided by the sysroot).  Disables oniguruma regex support to
# minimise dependencies.

NAME    := jq
VERSION := 1.7.1
CATEGORY:= textproc
LICENSE := MIT
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET  := $(FREELINX_TRIPLE)
PREFIX  := /

INSTALL_BIN:= jq
INSTALL_RELPATH:= usr/bin/jq

FLX_PROJECT_LIBS :=
FLX_PROJECT_CPPFLAGS =
FLX_PROJECT_LDFLAGS  =

include ../../mk/project-port.mk

SRC_TREE = $(FREELINX_BUILD_DIR)/work/$(NAME)/jq-$(VERSION)
PROJECT_BIN = $(SRC_TREE)/jq
BUILD_BIN   = $(PROJECT_BIN)

PROJECT_CFG_CMDS = \
	./configure \
		--host=$(FREELINX_TRIPLE) \
		--prefix=/usr \
		--disable-shared \
		--enable-static \
		--without-oniguruma \
		CC="$(FREELINX_CC)" \
		CFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -O2" \
		LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -static -fuse-ld=lld --rtlib=compiler-rt"

BUILD_CMDS = \
	make -j"$(FREELINX_JOBS)" LDFLAGS="$(FREELINX_TARGET_FLAGS) $(FREELINX_SYSROOT_FLAGS) -all-static -fuse-ld=lld --rtlib=compiler-rt -lm"
