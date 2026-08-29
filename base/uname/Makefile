# FreeLinX/ports - base/uname : NetBSD 10.1 `/usr/bin/uname`.
#
# Builds the ACTUAL NetBSD uname utility source (usr.bin/uname/uname.c from
# the 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.
#
# OS porting notes (patches/patch-uname-sysctl-linux):
#   - NetBSD's `-p` (machine arch) reads the BSD-only CTL_HW.HW_MACHINE_ARCH
#     sysctl; Linux has no sysctl(2), so the patch derives it from uname(2)'s
#     utsname.machine (e.g. "x86_64") via strlcpy (base/compat).
#   - core fields (sysname/nodename/release/version/machine) come from
#     uname(2), which musl provides.
NAME   := uname
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-4-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/uname -> src/rootfs/bin/uname.
INSTALL_BIN:= uname
INSTALL_RELPATH:= bin/uname

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/uname/uname.c

COMPAT_SRCS = $(FLX_COMPAT)/strlcpy.c

PORT_SRCS = usr.bin/uname/uname.c
