# FreeLinX/ports - base/who : NetBSD 10.1 `/usr/bin/who`.
#
# who is built with the utmpx backend only (SUPPORT_UTMPX, not SUPPORT_UTMP):
# musl provides the utmpx database functions (setutxent/getutxent/utmpxname
# live in musl libc) and <utmpx.h>, whereas the BSD <utmp.h> path and struct
# don't exist on musl.  On Linux who reads the host's /var/run/utmp (via
# musl's utmpx layer); if that file is absent it simply lists nobody and exits
# 0 (acceptable).  -D_GNU_SOURCE exposes musl's utmpxname() prototype and the
# e_termination/e_exit member aliases for struct utmpx.
#
# musl struct utmpx uses `ut_user` where NetBSD uses `ut_name`, and musl
# st_mtim where NetBSD uses st_mtimespec, and musl has no _PATH_UTMPX macro, so
# utmpentry.c is patched for those plus the BSD timespecclear() helper.
NAME   := who
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= who
INSTALL_RELPATH:= bin/who

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/who/utmpentry.c usr/src/usr.bin/who/utmpentry.h \
	usr/src/usr.bin/who/who.c

FLX_CPPFLAGS += -D_GNU_SOURCE -DSUPPORT_UTMPX

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/who/utmpentry.c usr.bin/who/who.c
