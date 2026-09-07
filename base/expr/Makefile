# FreeLinX/ports - base/expr : NetBSD 10.1 `/usr/bin/expr`.
#
# Builds the ACTUAL NetBSD expr utility source (bin/expr/expr.y from the 10.1
# src set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# The source is a yacc grammar; a host bison is used to generate expr.c from
# expr.y (FLX_PREGEN), then the FreeLinX toolchain compiles and links it. No
# GNU runtime or tool in the system or the build.

NAME   := expr
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot), bison (host, for pregen)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/expr -> src/rootfs/bin/expr.
INSTALL_BIN:= expr
INSTALL_RELPATH:= bin/expr

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

# Extract the yacc grammar.
NETBSD_MEMBERS = usr/src/bin/expr/expr.y

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

# FLX_PREGEN runs after extraction/patching: bison (host) turns the grammar
# into expr.c inside SRC_DIR. PORT_SRCS then compiles that generated .c.
PORT_SRCS = bin/expr/expr.c

FLX_PREGEN = cd "$(SRC_DIR)" && bison -d -o bin/expr/expr.c bin/expr/expr.y
