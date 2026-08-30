# FreeLinX/ports - base/date : NetBSD 10.1 `/bin/date`.
#
# Built the NetBSD nbtool way (-DHAVE_NBTOOL_CONFIG_H=1): bin/date's source is
# structured with #if HAVE_NBTOOL_CONFIG_H guards that select the self-contained
# ISO-only -d parser (parse_iso_datetime) in place of the libutil parsedate() and
# that turn the time-setting path into a plain error, so no logwtmp(3)/
# parsedate(3) (BSD libutil) is pulled in.  The unavoidable BSD `<protocols/
# timed.h>` for netdate.c comes from the same src set (usr/src/include/protocols/
# timed.h) and is extracted to $(SRC_DIR)/include so <protocols/timed.h> resolves
# via the default -I$(SRC_DIR)/include.  <tzfile.h> is included but unused and
# does not exist on musl, so it is removed by patch.
#
# date still reads the clock, formats it (strftime), and honours $TZ / -u
# (setenv("TZ","UTC0",1)); only <setting> the system time is unsupported in the
# tools build (matches NetBSD's own nbtool build).
NAME   := date
VERSION:= 10.1
CATEGORY:= base
LICENSE:= BSD-2-Clause
DEPENDENCIES:=
BUILD_DEPENDENCIES:= musl (via FreeLinX toolchain sysroot)
TARGET := $(FREELINX_TRIPLE)
PREFIX := /

INSTALL_BIN:= date
INSTALL_RELPATH:= bin/date

include ../../mk/base-port.mk

SRC_DIR  = $(FREELINX_BUILD_DIR)/work/$(NAME)
OBJ_DIR  = $(FREELINX_BUILD_DIR)/obj/$(NAME)
BUILD_BIN = $(SRC_DIR)/$(NAME)

NETBSD_MEMBERS = usr/src/bin/date/date.c usr/src/bin/date/extern.h \
	usr/src/bin/date/netdate.c usr/src/include/protocols/timed.h

FLX_CPPFLAGS += -DHAVE_NBTOOL_CONFIG_H=1

COMPAT_SRCS = $(FLX_COMPAT)/getprogname.c

PORT_SRCS = bin/date/date.c bin/date/netdate.c
