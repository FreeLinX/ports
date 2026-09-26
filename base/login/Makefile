# FreeLinX/ports - base/login : NetBSD 10.1 `/usr/bin/login`.
#
# NetBSD builds login two ways, chosen in its own Makefile by USE_PAM:
#
#   USE_PAM != no   login_pam.c   - all authentication goes through PAM
#   USE_PAM == no   login.c      - the traditional path: authenticate against
#                                  /etc/passwd + /etc/shadow directly, and
#                                  enforce the account/password expiry policy
#                                  the password database carries
#
# FreeLinX builds the second one.  There is no PAM in the tree: the only thing
# under build/deps/openpam is an empty include/security and an empty lib, so
# the first path would not build and, if it were made to build by stubbing
# libpam out, would authenticate against a stack that does nothing.  The
# traditional path is a complete implementation - it is what NetBSD shipped for
# decades - and it needs only the two files below.
#
# When a real libpam lands, switching to it is: add its include and archive to
# this Makefile, put login_pam.c back in PORT_SRCS, and delete the two shims
# below (login_pam.c uses pam_get_item/pam_authenticate, not struct passwd's
# expiry fields).  Nothing else changes.
NAME   := login
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= login
INSTALL_RELPATH:= bin/login

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/login/common.c \
	usr/src/usr.bin/login/common.h \
	usr/src/usr.bin/login/k5login.c \
	usr/src/usr.bin/login/login.c \
	usr/src/usr.bin/login/pathnames.h

PORT_SRCS = usr.bin/login/common.c \
	usr.bin/login/k5login.c \
	usr.bin/login/login.c

# musl's getpwnam(3) reads /etc/passwd, whose hash field is "x" on a FreeLinX
# system, and its struct passwd has no pw_expire/pw_change and cannot be
# extended to carry them.  A login built on it alone compares crypt(p, "x")
# against "x" and rejects every password, so this port reads the hash, the lock
# state and the two expiry policies out of /etc/shadow itself.  See
# shadow_auth.c.
COMPAT_SRCS += $(CURDIR)/shadow_auth.c

# setlogin(3)/ttyaction(3), which musl has no equivalent of.  See
# base/compat/login_ttyaction.c for why ttyaction() succeeding is the honest
# answer rather than a stub that would make login(1) warn on every login.
COMPAT_SRCS += $(FLX_COMPAT)/login_ttyaction.c

# The rest are already written and already used by other ports; login is simply
# the first caller of all four at once:
#   sigstub.c       sockaddr_snprintf(3), for the remote-host line in syslog
#   user_from_uid.c user_from_uid(3), to name the owner of the tty
#   unvis.c         strunvis(3), for the untrusted-host address in that line
#   copyrightstr.c  the notice NetBSD's login prints at the top of a console
#                   login; NetBSD generates it from sys/conf/copyright with awk
COMPAT_SRCS += $(FLX_COMPAT)/sigstub.c
COMPAT_SRCS += $(FLX_COMPAT)/user_from_uid.c
COMPAT_SRCS += $(FLX_COMPAT)/unvis.c
COMPAT_SRCS += $(CURDIR)/copyrightstr.c

# so that login.c's #include "shadow_auth.h" resolves
FLX_CPPFLAGS += -I$(CURDIR)
