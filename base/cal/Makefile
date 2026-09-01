# FreeLinX/ports - base/cal : NetBSD 10.1 `/usr/bin/cal`.
#
# Builds the ACTUAL NetBSD cal utility source (usr.bin/cal/cal.c from the
# 10.1 src set) with the FreeLinX toolchain (clang + LLD + musl sysroot),
# static.  Not an invented cal or a host coreutils binary.
#
# OS porting notes (see patches/patch-cal-musl):
#   - `<term.h>`/`<tzfile.h>` are absent from musl.  tzfile.h only supplies
#     TM_YEAR_BASE (defined locally); term.h was used solely to reverse/bold
#     the "today" highlight via setupterm(3) -- there is no terminfo library
#     in the musl sysroot, so the init_hilite() terminfo path is stubbed out.
#     The `-h` flag still works (highlights today via a double-print
#     backspace trick); it just no longer changes terminal attributes.
NAME   := cal
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= cal
INSTALL_RELPATH:= bin/cal

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/usr.bin/cal/cal.c

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = usr.bin/cal/cal.c