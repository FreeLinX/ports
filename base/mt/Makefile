# FreeLinX/ports - base/mt : NetBSD 10.1 `/sbin/mt`.
#
# Builds the ACTUAL NetBSD mt source (bin/mt/mt.c from the 10.1 src set) with
# the FreeLinX toolchain (clang + LLD + musl sysroot), static.
#
# OS porting notes:
#   - setprogname()/getprogname() come from base/compat/getprogname.c;
#     err(3)/errx(3) come from musl's <err.h>.
#   - BLOCKED-ON-UPSTREAM: mt.c drives NetBSD tape storage via the BSD MTIO
#     ioctl interface (MTIOCTOP/MTIOCGET with struct mtop/mtget) and rmt(8).
#     Linux tape devices are controlled through the Linux <sys/mtio.h>
#     interface/ioctls instead.  The port compiles against the Linux MTIO
#     headers that musl ships, but the NetBSD tape commands (e.g. rewind,
#     offline via a BSD mtop) do not map 1:1 onto Linux ioctls.  A Linux
#     sys/tape shim (LOCAL_SRCS backend translating the BSD commands to Linux
#     MTIOCTOP/MTIOCGET) is needed before this is fully functional on Linux.
NAME   := mt
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/sbin/mt -> src/rootfs/sbin/mt (NetBSD puts mt in /sbin).
INSTALL_BIN:= mt
INSTALL_RELPATH:= sbin/mt

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/mt/mt.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = bin/mt/mt.c