# FreeLinX/ports - base/ftp : NetBSD 10.1 `/usr/bin/ftp`.
#
# Builds the ACTUAL NetBSD 10.1 ftp client source (usr.bin/ftp/{cmds.c,
# cmdtab.c, complete.c, domacro.c, fetch.c, ftp.c, main.c, progressbar.c,
# ruserpass.c, util.c} plus the include/stringlist.h and stringlist.c it needs)
# with the FreeLinX toolchain (clang + LLD + musl sysroot), static.  Not an
# invented client or a host distribution's ftp.
#
# Build configuration:
#   - SMALLPROG mode (the NetBSD Makefile's own minimal-user configuration)
#     disables line editing, help, about, auth, usage, and status footers:
#     `ftp` then links libc only - no libedit, no libterminfo.
#   - SSL (WITH_SSL, libssl+libcrypto) is NOT enabled: uhttp/fetch's TLS paths
#     are #ifdef WITH_SSL and stay out, so the binary has no crypto dependency.
#   - INET6 is kept (IPv6 support where practical), matching the NetBSD
#     Makefile's SMALLPROG_INET6 switch.
#
# libc notes: musl provides getpass(3), glob(3), vasprintf(3), timegm(3),
# strlcpy(3).  The NetBSD sources use getprogname/setprogname (compat),
# reallocarr (compat, for libc-gen stringlist.c) and <stringlist.h> +
# <tzfile.h> which musl lacks - both extracted verbatim from the same
# NetBSD 10.1 src set (include/).  progressbar.c's remaining-time formatting
# uses SECSPERHOUR from NetBSD's <tzfile.h>.
NAME   := ftp
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= ftp
INSTALL_RELPATH:= bin/ftp

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/ftp/cmds.c usr/src/usr.bin/ftp/cmdtab.c \
	usr/src/usr.bin/ftp/complete.c usr/src/usr.bin/ftp/domacro.c \
	usr/src/usr.bin/ftp/extern.h usr/src/usr.bin/ftp/fetch.c \
	usr/src/usr.bin/ftp/ftp.c usr/src/usr.bin/ftp/ftp_var.h \
	usr/src/usr.bin/ftp/main.c usr/src/usr.bin/ftp/progressbar.c \
	usr/src/usr.bin/ftp/progressbar.h usr/src/usr.bin/ftp/ruserpass.c \
	usr/src/usr.bin/ftp/ssl.c usr/src/usr.bin/ftp/ssl.h usr/src/usr.bin/ftp/util.c \
	usr/src/usr.bin/ftp/version.h \
	usr/src/include/stringlist.h usr/src/lib/libc/gen/stringlist.c \
	usr/src/include/tzfile.h

# ssl.c contains the plain socket/file transport too; WITH_SSL only controls
# its optional OpenSSL calls.  Keep it so HTTP and FTP transfer helpers link.
PORT_SRCS = usr.bin/ftp/cmds.c usr.bin/ftp/cmdtab.c usr.bin/ftp/complete.c \
	usr.bin/ftp/domacro.c usr.bin/ftp/fetch.c usr.bin/ftp/ftp.c \
	usr.bin/ftp/main.c usr.bin/ftp/progressbar.c usr.bin/ftp/ruserpass.c \
	usr.bin/ftp/util.c usr.bin/ftp/ssl.c lib/libc/gen/stringlist.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(FLX_COMPAT)/reallocarr.c

# Minimal-user configuration, exactly the defines the NetBSD Makefile adds
# under SMALLPROG, plus INET6 (its SMALLPROG_INET6 switch):
#   -DNO_EDITCOMPLETE: complete.c/util.c editline paths and <histedit.h> stay out
#   -DNO_ABOUT -DNO_AUTH -DNO_HELP -DNO_STATUS -DNO_DEBUG -DNO_USAGE
# stringlist.h/tzfile.h (extracted, stripped) land in $(SRC_DIR)/include,
# reachable through the default FLX_CPPFLAGS include path.
FLX_CPPFLAGS += -DSMALLPROG -DSMALLPROG_INET6 -DINET6 \
	-DNO_EDITCOMPLETE -DNO_ABOUT -DNO_AUTH -DNO_HELP \
	-DNO_STATUS -DNO_DEBUG -DNO_USAGE -include signal.h -DINFTIM=-1
