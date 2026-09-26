# FreeLinX/ports - base/timeout : NetBSD 10.1 `/usr/bin/timeout`.
NAME   := timeout
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /
INSTALL_BIN:= timeout
INSTALL_RELPATH:= bin/timeout

include ../../mk/base-port.mk

# base/timeout/sys_signame.c builds its own table from musl's signal numbers, so
# it names the signals FreeLinX actually delivers rather than BSD numbering.  The
# forced-include header declared a const-qualified array of a different size, and
# "const char *[]" against "const char *const[NSIG+1]" is a hard error.
FLX_CPPFLAGS += -DFLX_BSD_SKIP_DECL_SYS_SIGNAME=1

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/timeout/timeout.c
PORT_SRCS = usr.bin/timeout/timeout.c

# timeout.c reads the NetBSD globals sys_nsig and sys_signame[]; musl provides
# neither.  A FreeLinX compat shim (this port dir) supplies them from musl's
# own <signal.h> macros.
COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c $(CURDIR)/sys_signame.c
