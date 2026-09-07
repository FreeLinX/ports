# FreeLinX/ports - base/domainname : NetBSD 10.1 `/bin/domainname`.
#
# Builds the ACTUAL NetBSD domainname utility source (bin/domainname/
# domainname.c from the 10.1 src set) with the FreeLinX toolchain (clang +
# LLD + musl sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() come from base/compat/getprogname.c.
#   - getdomainname()/setdomainname()/MAXHOSTNAMELEN are POSIX, provided by
#     musl (<sys/param.h> defines MAXHOSTNAMELEN; the libc calls are in
#     unistd.h).
NAME   := domainname
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/domainname -> src/rootfs/bin/domainname.
INSTALL_BIN:= domainname
INSTALL_RELPATH:= bin/domainname

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/domainname/domainname.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = bin/domainname/domainname.c
