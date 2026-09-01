# FreeLinX/ports - base/chown : NetBSD 10.1 `/usr/sbin/chown`.
#
# Builds the ACTUAL NetBSD chown source (sbin/chown/chown.c from the 10.1 src
# set) with the FreeLinX toolchain (clang + LLD + musl sysroot), static.  It
# is not an invented chown or a host coreutils binary.
#
# libc pieces chown pulls in, all extracted from the same src set: fts(3)
# (lib/libc/gen/fts.c + include/fts.h, built the nbtool way) and pwcache(3)
# (lib/libc/gen/pwcache.c + pwcache.h) for uid_from_user(3)/gid_from_group(3),
# which chown's a_uid()/a_gid() use to resolve owner:group names.
#
# OS porting notes:
#   - SUPPORT_DOT (BSD-only `user.group` syntax) is left undefined, so the
#     uid_from_user(3) block compiles out; the standard `owner:group` syntax
#     is handled by chown's own a_uid()/a_gid() code.
#   - setprogname/getprogname (used to detect `chown` vs `chgrp` semantics)
#     come from base/compat.
#   - patches/patch-chown-decls adds the uid_from_user(3)/gid_from_group(3)
#     prototypes (NetBSD declares them in <pwd.h>/<grp.h>, musl does not).
#   - patches/patch-pwcache-null-funcs points pwcache's optional
#     setpassent(3)/setgroupent(3) hooks at NULL: musl has neither.
#   - The same built binary is also installed as `chgrp` (via
#     INSTALL_ALIASES); chown.c's argv[0]-based `ischown` (myname[2]=='o')
#     selects chgrp semantics when invoked through that name.
NAME   := chown
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

# Installed as staging/bin/chown.  Set before include.
INSTALL_BIN:= chown
INSTALL_RELPATH:= bin/chown

# Install the same binary as bin/chgrp too (argv[0]-driven front-end).
INSTALL_ALIASES:= chgrp

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/sbin/chown/chown.c \
	usr/src/lib/libc/gen/fts.c \
	usr/src/lib/libc/gen/pwcache.c usr/src/lib/libc/gen/pwcache.h \
	usr/src/include/fts.h

# fts.c built the nbtool way.
FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

# musl lacks setprogname/getprogname (base/compat) and fts(3) needs
# reallocarr(3) (base/compat).
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c

PORT_SRCS = sbin/chown/chown.c lib/libc/gen/fts.c lib/libc/gen/pwcache.c