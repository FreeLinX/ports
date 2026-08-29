# FreeLinX/ports - base/env : NetBSD 10.1 `/usr/bin/env`.
#
# Builds the ACTUAL NetBSD env utility source (usr.bin/env/env.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  It is not an invented env or a host coreutils binary.
#
# OS porting notes (see patches/README):
#   - setprogname()/getprogname() come from base/compat/getprogname.c.
#   - everything else env.c uses (err(3), setlocale, getopt, putenv,
#     unsetenv, execvp, chdir) is plain POSIX that musl provides.
NAME   := env
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-3-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/env -> src/rootfs/bin/env.  Set before include.
INSTALL_BIN:= env
INSTALL_RELPATH:= bin/env

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/env/env.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/env/env.c
