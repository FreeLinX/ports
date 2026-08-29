# FreeLinX/ports - base/mv : NetBSD 10.1 `/bin/mv`.
#
# Builds the ACTUAL NetBSD mv utility source (bin/mv/mv.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.
# It is not an invented mv or a host coreutils binary.
#
# OS porting notes (see patches/README):
#   - patches/patch-mv-linux removes the unused <sys/extattr.h> include
#     (Linux has no extattr(2)) and compiles out the BSD-only "copy file
#     flags" step (fchflags/st_flags), which has no musl/Linux equivalent.
NAME   := mv
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/mv -> src/rootfs/bin/mv.  Set before include.
INSTALL_BIN:= mv
INSTALL_RELPATH:= bin/mv

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/mv/mv.c usr/src/bin/mv/pathnames.h

# musl lacks strlcpy(3), user_from_uid(3)/group_from_gid(3) (mv's -i prompt)
# and strmode(3); supplied from base/compat.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/strlcpy.c \
	$(FLX_COMPAT)/strmode.c $(FLX_COMPAT)/user_from_uid.c

PORT_SRCS = bin/mv/mv.c