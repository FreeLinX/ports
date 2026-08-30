# FreeLinX/ports - base/test : NetBSD 10.1 `/bin/test`.
#
# Builds the ACTUAL NetBSD test utility source (bin/test/test.c from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes (see patches/README):
#   - none required beyond the module itself: test.c's BSD-isms (getprogname,
#     setprogname, timespeccmp for the -nt/-ot/-ef file comparisons) come
#     from the FreeLinX musl-compat layer; it already names the musl
#     st_mtim member, so no struct patch is needed.

NAME   := test
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/test -> src/rootfs/bin/test.
INSTALL_BIN:= test
INSTALL_RELPATH:= bin/test

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/test/test.c

# Default compat (getprogname.c) provides getprogname(3)/setprogname(3),
# and compat <sys/cdefs.h> provides timespeccmp().  Nothing more needed.
PORT_SRCS = bin/test/test.c
