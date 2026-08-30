# FreeLinX/ports - base/nohup : NetBSD 10.1 `/usr/bin/nohup`.
#
# Builds the ACTUAL NetBSD nohup utility source (usr.bin/nohup/nohup.c from
# the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes: nohup.c builds its $HOME/nohup.out path with the BSD
# strlcpy(3)/strlcat(3) pair (musl omits them); both come from
# base/compat/strlcpy.c.  __RCSID/__dead and MAXPATHLEN are covered by the
# FreeLinX compat <sys/cdefs.h> and musl's <sys/param.h>.
NAME   := nohup
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/nohup -> src/rootfs/bin/nohup.
INSTALL_BIN:= nohup
INSTALL_RELPATH:= bin/nohup

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/nohup/nohup.c

# musl lacks strlcpy(3)/strlcat(3) (BSD libc); from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c

PORT_SRCS = usr.bin/nohup/nohup.c